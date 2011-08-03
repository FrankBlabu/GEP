/*
 * trace_guard.cpp - Wrapper for backtrace catching
 *
 * Frank Cieslok, 20.06.2006
 */

#include "GEPTraceGuard.hpp"

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

namespace GEP {

  //#*************************************************************************
  // CLASS GEP::TraceGuard
  //#*************************************************************************

  /* Constructor */
  TraceGuard::TraceGuard (const QString& application_name)
    : _application_name (application_name),
      _main_pid         (getpid ()),
      _error_mode       (BT)
  {
  }

  /* Set error mode */
  void TraceGuard::setErrorMode (ErrorMode_t error_mode)
  {
    _error_mode = error_mode;
  }
  
  /* Handle incoming exception */
  void TraceGuard::error (const Exception& exception)
  {
    switch (_error_mode)
    {
      case NONE:
	abort ();
	break;

      case BT:
	printBacktrace ();
	break;

      case GDB:
      {
	QString command = QString ("gdb -q %2% %1%")
	  .arg (getpid ())
	  .arg (_application_name);
	system (qPrintable (command));
      }
      break;

      case DDD:
      {
	QString command = QString ("ddd %2% %1%")
	  .arg (getpid ())
	  .arg (_application_name);
	system (qPrintable (command));
      }
      break;
    }
  }

  /* Print text to stderr */
  void TraceGuard::printError (const QString& text)
  {
    std::cerr << qPrintable (text) << std::endl;
    std::cerr.flush ();
  }

  
  /* Print backtrace via GDB */
  void TraceGuard::printBacktrace ()
  {
    printError (QString ("\nMain PID: %1%\nStacktrace:").arg (_main_pid));
    
    int handle[3][2];
    
    if (pipe (handle[0]) < 0)
      {
	printError ("Could not create pipe 0");
	abort ();
      }

    if (pipe (handle[1]) < 0)
      {
	printError ("Could not create pipe 1");
	abort ();
      }

    if (pipe (handle[2]) < 0)
      {
	printError ("Could not create pipe 2");
	abort ();
      }

    //
    // Start GDB in a seperate process
    //
    pid_t gdb_pid = fork ();
    
    if (gdb_pid < 0)
      {
	printError ("fork () failed");
	abort ();
      }

    if (gdb_pid == 0)
      {
	close (handle[0][1]);
	close (handle[1][0]);
	close (handle[2][0]);
	
	if (dup2 (handle[0][0], 0) < 0)
	  exit (0);
	if (dup2 (handle[1][1], 1) < 0)
	  exit (0);
	if (dup2 (handle[2][1], 2) < 0)
	  exit (0);

	execlp ("gdb", "gdb", "-q", NULL);
	printError ("gdb finished without exit...");
	exit (0);
      }

    close (handle[0][0]);
    close (handle[1][1]);
    close (handle[2][1]);

    //
    // A second child process is used to control GDB
    //
    pid_t control_pid = fork ();
    
    if (control_pid < 0)
      {
	printError ("couldn't fork for gdb control");
	kill (gdb_pid, SIGKILL);
	close (handle[0][1]);
	close (handle[1][0]);
	close (handle[2][0]);
	exit (0);
      }

    if (control_pid == 0)
      {
	controlGDB (gdb_pid, handle[0][1], handle[1][0], handle[2][0]);
	printError ("gdb control finished without exit...");
	exit (0);
      }

    close (handle[0][1]);
    close (handle[1][0]);
    close (handle[2][0]);

    for (pid_t err = -1; err != control_pid; err = waitpid (control_pid, 0, 0))
    {
      if (err < 0)
      {
	printError ("error while waiting for gdb control -- exit");
	abort ();
      }
    }
    
    printError ("finished stacktrace -- exit");
    exit (1);
  }

#define S_MAX 100
#define OUT_MAX 4096
#define GM_MAX 100000

  /*!
   * Control externally running GDB process
   */
  void TraceGuard::controlGDB (int /*gdb_pid*/, int /*gdb_stdin*/,
			       int /*gdb_stdout*/, int /*gdb_stderr*/)
  {
#if 0
    int state = 1;
    
    int threads[50];
    uint threads_count = 0;
    int thread_pids[50];
    uint thread_pids_count = 0;    
    uint act_thread = 0;
    
    QString out;
    
    bool empty_command = false;
    bool have_prompt = false;
    int bigest = max (gdb_stdout, gdb_stderr) + 1;
    
    char gdb_messages[GM_MAX];
    uint gdb_messages_count = 0;

    //
    // Do until all states have been processed or the debugger has been
    // terminated
    //
    while (state >= 0 && waitpid (gdb_pid, NULL, WNOHANG) != gdb_pid)
    {
      if (!empty_command)
      {
	fd_set rdfds;
	FD_ZERO (&rdfds);
	FD_SET (gdb_stdout, &rdfds);
	FD_SET (gdb_stderr, &rdfds);
	
	timeval timeout;
	timeout.tv_sec = 60;
	timeout.tv_usec = 0;

	int no = select (bigest, &rdfds, NULL, NULL, &timeout);
	if (no < 0)
	{
	  if (errno == EINTR)
	    continue;

	  //
	  // If 'select ()' failed we are doomed
	  //
	  printError ("Error in select: ", strerror (errno));
	  abort ();
	}

	if (no == 0)
	{
	  printError ("Timeout while waiting for gdb");
	  abort ();
	}

	//
	// Fetch gdb error message
	//
	if (FD_ISSET (gdb_stderr, &rdfds))
	{
	  int n = read (gdb_stderr, &gdb_messages[gdb_messages_count], 1024);
	  if (n < 0)
	    continue;
	  gdb_messages_count += n;
	  gdb_messages[gdb_messages_count] = 0;
		
	  continue;
	}

	//
	// Fetch result line by line, because there is no prompt at 'new line'
	//
	have_prompt = false;

	char c;
	read (gdb_stdout, &c, 1);

	if (c != '\n')
	{
	  out += c;

	  if (out != "gdb")
	    continue;
	  
	  have_prompt = true;
	}
	else
	{
		addCharBuf (gdb_messages, gdb_messages_count, GM_MAX, out);
		addChar (gdb_messages, gdb_messages_count, GM_MAX, '\n');
		DV (("eol"));
	      }
	  }
	else
	  {
	    DV (("empty cmd"));
	    have_prompt = true;
	    empty_command = false;
	  }

	DV (("gdb: \"%s\"", out));
	DV (("**** state %d", state));

	switch (state)
	  {
	  case 0:
	    if (have_prompt)
	      {
		DV (("exec-file"));
		write (gdb_stdin, "exec-file ", 10);
		write (gdb_stdin, prog_name.c_str (), prog_name.length ());
		write (gdb_stdin, "\n", 1);
		fsync (gdb_stdin);
		addCharBuf (gdb_messages, gdb_messages_count, GM_MAX,
			    "(gdb) exec-file ");
		addCharBuf (gdb_messages, gdb_messages_count, GM_MAX,
			    prog_name.c_str ());
		addChar (gdb_messages, gdb_messages_count, GM_MAX, '\n');
		state++;
	      }
	    break;
	    
	  case 1:
	    if (have_prompt)
	      {
		DV (("set confirm off"));
		write (gdb_stdin, "set confirm off\n", 16);
		fsync (gdb_stdin);
		addCharBuf (gdb_messages, gdb_messages_count, GM_MAX,
			    "(gdb) set confirm off");
		addChar (gdb_messages, gdb_messages_count, GM_MAX, '\n');
		state++;
	      }
	    break;
	    
	  case 2:
	    if (have_prompt)
	      {
		DV (("attach %d", main_pid));
		char s[S_MAX];
		uint s_count = 0;
		addCharBuf (s, s_count, S_MAX, "attach ");
		addInt (s, s_count, S_MAX, main_pid);
		addChar (s, s_count, S_MAX, '\n');
		write (gdb_stdin, s, s_count);
		fsync (gdb_stdin);
		addCharBuf (gdb_messages, gdb_messages_count, GM_MAX,
			    "(gdb) attach ");
		addInt (gdb_messages, gdb_messages_count, GM_MAX, main_pid);
		addChar (gdb_messages, gdb_messages_count, GM_MAX, '\n');
		state++;
		DV (("sent"));
	      }
	    break;

	  case 3:
	    if (have_prompt)
	      {
		DV (("info threads"));
		write (gdb_stdin, "info threads\n", 13);
		fsync (gdb_stdin);
		addCharBuf (gdb_messages, gdb_messages_count, GM_MAX,
			    "(gdb) info threads");
		addChar (gdb_messages, gdb_messages_count, GM_MAX, '\n');
		state++;
	      }
	    break;

	  case 4:
	    if (!have_prompt)
	      {
		DV (("out: '%s'", out));
		char *c = out;
		int thread_no = strtol (c, &c, 10);
		while (*c == ' ')
		  ++c;
		while (*c != ' ' && *c != '\0')
		  ++c;
		/* unused */ strtol (c, &c, 10);
		while (*c == ' ')
		  ++c;
		while (*c != ' ' && *c != '\0')
		  ++c;
		int th_pid = strtol (c, 0, 10);
		
		if (thread_no > 0)
		  {
		    DV (("Thread %d found pid %d",
			 thread_no, th_pid));
		    threads[threads_count++] = thread_no;
		    thread_pids[thread_pids_count++] = th_pid;
		  }
		
		break;
	      }
	    DV (("threads read: %d", threads_count));
	    state++;
	    empty_command = true;
	    break;
	    
	  case 5:
	    if (act_thread < threads_count)
	      {
		if (have_prompt)
		  {
		    cerrCharBuf ("\nThread ");
		    cerrInt (threads[act_thread]);
		    cerrCharBuf (" PID ");
		    cerrInt (thread_pids[act_thread]);
		    cerrChar ('\n');
		    DV (("thread %d", threads[act_thread]));
		    char s[S_MAX];
		    uint s_count = 0;
		    addCharBuf (s, s_count, S_MAX, "thread ");
		    addInt (s, s_count, S_MAX, threads[act_thread]);
		    addChar (s, s_count, S_MAX, '\n');
		    write (gdb_stdin, s, s_count);
		    fsync (gdb_stdin);
		    addCharBuf (gdb_messages, gdb_messages_count, GM_MAX,
				"(gdb) thread ");
		    addInt (gdb_messages, gdb_messages_count, GM_MAX,
			    threads[act_thread]);
		    addChar (gdb_messages, gdb_messages_count, GM_MAX, '\n');
		    act_thread++;
		    state++;
		  }
		break;
	      }
	    DV (("end"));
	    state = 8;
	    empty_command = true;
	    break;

	  case 6:
	    if (have_prompt)
	      {
		DV (("bt"));
		write (gdb_stdin, "bt 100\n", 7);
		fsync (gdb_stdin);
		addCharBuf (gdb_messages, gdb_messages_count, GM_MAX,
			    "(gdb) bt 100\n");
		state++;
	      }
	    break;

	  case 7:
	    if (have_prompt)
	      {
		DV (("empty statement"));
		empty_command = true;
		state = 5;
	      }
	    else
	      {
		cerrCharBuf (out);
		cerrChar ('\n');
	      }
	    break;

	  case 8:
	    if (have_prompt)
	      {
		DV (("detach"));
		write (gdb_stdin, "detach\n", 7);
		fsync (gdb_stdin);
		addCharBuf (gdb_messages, gdb_messages_count, GM_MAX,
			    "(gdb) detach\n");
		state++;
	      }
	    break;

	  case 9:
	    if (!have_prompt)
	      break;

	  default:
	    // Normales Ende
	    state = -1;
	    break;
	  } // switch (state)

	out_count = 0;
      } // while (state >= 0)

    if (state != -1)
      {
	cerrCharBuf ("\nProblems with gdb in state ");
	cerrInt (state);
	cerrCharBuf (", session log follows:\n");
	cerrCharBuf (gdb_messages);
	cerrCharBuf ("\nend of gdb session log.\n");
      }
    else
      {
	cerrCharBuf ("\nfinished stacktrace -- exit\n");
      }

    DV (("good bye"));
    int self_pid = ::getpid ();

    for (uint i=0; i < thread_pids_count; ++i)
      if (thread_pids[i] != 0)
	kill (thread_pids[i], SIGKILL);

    kill (gdb_pid, SIGKILL); // gdb

    // Falls gdb nicht die threads zurückgeliefert hatte:
    kill (main_pid, SIGKILL);
    kill (thread_pid, SIGKILL);

    // Damit keine weiteren Ausgaben vom Speichercheck oder ImageCache
    // (Tritop) kommen:
    kill (self_pid, SIGKILL); // suicide
    // Ansonsten:
    return;

#endif
  }

} // namespace GEP
