#include <sys/inotify.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include "utils.h"

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;


// display information from inotify_event structure
void displayInotifyEvent(struct inotify_event* e);


int notifyId = -1;
int childPid = -1;
char *childArgv[2] = {"", nullptr};
char *childEnvp[2] = {"", nullptr}; 
int numRead;
size_t const NAME_MAX = 4096;
size_t const BUF_SIZE = 10 * (sizeof(struct inotify_event) + NAME_MAX + 1);
char eventsBuf[BUF_SIZE];
char *cursor;
struct inotify_event* event;
int error;
string filename;
string absname;
string projectDir;
string staticDir;


int runChild(char const* app);
void showError();


int monitor() {
  cout << "monitor childPid: " << childPid << endl;

  // inotify init
  notifyId = inotify_init();
  /* int notifyId = inotify_init1(IN_CLOEXEC);// | IN_NONBLOCK); */
  if (notifyId == -1) {
    cout << "inotify_init error" << endl;
    return -1;
  }

  /* int watchId = inotify_add_watch(notifyId, filename.c_str(), IN_MODIFY); */
  /* if (watchId == -1) { */
  /*   cout << "inotify_add_watch error" << endl; */
  /*   return -1; */
  /* } */
  int projectId = inotify_add_watch(notifyId, projectDir.c_str(), IN_MOVED_TO | IN_DELETE);
  if (projectId == -1) {
    cout << "inotify_add_watch projectId error" << endl;
    showError();
    return -1;
  }
  /* cout << "projectId: " << projectId << endl; */

  int staticId = inotify_add_watch(notifyId, staticDir.c_str(), IN_CLOSE_WRITE);// IN_MOVED_TO | IN_DELETE);
  if (staticId == -1) {
    cout << "inotify_add_watch staticId error" << endl;
    showError();
    return -1;
  }
  /* cout << "staticId: " << staticId << endl; */

  /* int watchId; */
  /*   watchId = inotify_add_watch(notifyId, filename.c_str(), IN_ONESHOT | IN_IGNORED);//IN_MODIFY); */
  /*   if (watchId == -1) { */
  /*     cout << "inotify_add_watch error" << endl; */
  /*     cout << "errno: "; */
  /*     if (errno & EACCES) cout << "eacces\n"; */
  /*     if (errno & EBADF) cout << "ebadf\n"; */
  /*     if (errno & EFAULT) cout << "efault\n"; */
  /*     if (errno & EINVAL) cout << "einval\n"; */
  /*     if (errno & ENOMEM) cout << "enomem\n"; */
  /*     if (errno & ENOSPC) cout << "enospc\n"; */
  /*     cout << errno << endl; */
  /*     return -1; */
  /*   } */



  // epoll event loop
  // TODO: make epoll like in shot

  bool isChanges = false;
  for (;;) { // read events forever
    // inotofy add watch

    numRead = read(notifyId, eventsBuf, BUF_SIZE);
    if (numRead == 0) {
      cout << "read() from notifyId returned 0!" << endl;
      return -1;
    }
    if (numRead == -1) {
      cout << "read error" << endl;
      return -1;
    }

    /* cout << "read " << numRead << " bytes from notifyId" << endl; */

    isChanges = false;

    // process all of the events in buffer
    for (cursor = eventsBuf; cursor < eventsBuf + numRead; ) {
      event = (struct inotify_event*)(cursor);
      cursor += sizeof(struct inotify_event) + event->len;
      /* displayInotifyEvent(event); */

      if (event->wd == projectId) {
        if (event->name == filename) {
          /* if (event->mask & IN_DELETE) { */
          /*   /1* cout << "kill " << filename << endl; *1/ */
          /*   kill(childPid,  SIGKILL); */
          /*   waitpid(childPid, 0, 0); */
          /* } */
          /* if (event->mask & IN_MOVED_TO) { */
          /*   /1* cout << "start " << filename << endl; *1/ */
          /*   runChild(absname.c_str()); */
          /* } */
          isChanges = true;
        }
      } else if (event->wd == staticId) {
        isChanges = true;
      }
    }

    if (isChanges) {
      kill(childPid, SIGKILL);
      waitpid(childPid, 0, 0);
      sleep(1);
      runChild(absname.c_str());
    }
  }

  /* error = inotify_rm_watch(notifyId, event->wd); */
  /* if (error != 0) { */
  /*   if (error == -1) { */
  /*     cout << "initofy_rm_watch error" << endl; */
  /*     return -1; */
  /*   } */
  /* } */
  return 0;
}


int runChild(char const* app) {
  childPid = fork();
  int error;
  switch (childPid) {
    case -1:
      cout << "error run child app: " << app << endl;
      return -1;
    case 0:
      error = execve(app, childArgv, childEnvp);
      return 0;
    default:
      return monitor();
  }
  return 0;
}


int main(int argc, const char *argv[]) {
  filename = "server";
  projectDir = getCurrentDir();

  ostringstream buf;
  buf << projectDir << '/' << filename;
  absname = buf.str();

  staticDir = projectDir + "/static";

  if (!existsFile(absname)) {
    cout << "not exist app filename: " << filename << endl;
    return -1;
  }

  /* cout << "monitor file: " << absname << endl; */
  /* cout << "monitor projectDir: " << projectDir << endl; */
  /* cout << "monitor staticDir: " << staticDir << endl; */

  return runChild(absname.c_str());

  /* int childPid = fork(); */
  /* int appId = 0; */

  /* switch (childPid) { */
  /*   case -1: */
  /*     cout << "fork app error" << endl; */
  /*     return -1; */
  /*   case 0: */
  /*     cout << "child app enter" << endl; */
  /*     /1* /2* cout << "before system call" << endl; *2/ *1/ */
  /*     /1* appId = system(absname.c_str()); *1/ */
  /*     /1* switch (appId) { *1/ */
  /*     /1*   case -1: *1/ */
  /*     /1*     cout << "error run app\n"; *1/ */
  /*     /1*     return -1; *1/ */
  /*     /1*   case 0: *1/ */
  /*     /1*     cout << "error run app - no shell available\n"; *1/ */
  /*     /1*     return -1; *1/ */
  /*     /1*   default: *1/ */
  /*     /1*   cout << "after system call, appId: " << appId << endl; *1/ */
  /*     /1*   cout << "child app exit" << endl; *1/ */
  /*     /1*   return 0; *1/ */
  /*     /1* } *1/ */
  /*     break; */
  /*   default: */

  /*     break; */
  /* } // end of switch childPid */
  
  /* return 0; */
}


void showError() {
  cout << "errno: ";
  if (errno & EACCES) cout << "eacces, ";
  if (errno & EBADF) cout << "ebadf, ";
  if (errno & EFAULT) cout << "efault, ";
  if (errno & EINVAL) cout << "einval, ";
  if (errno & ENOMEM) cout << "enomem, ";
  if (errno & ENOSPC) cout << "enospc, ";
  cout << errno << endl;
}


void displayInotifyEvent(struct inotify_event* e) {
  cout << "wd: " << e->wd << endl;

  if (e->cookie > 0) {
    cout << "cookie: " << e->cookie << endl;
  }

  cout << "mask: ";
  if (e->mask & IN_ACCESS) cout << "IN_ACCESS ";
  if (e->mask & IN_ATTRIB) cout << "IN_ATTRIB ";
  if (e->mask & IN_CLOSE_NOWRITE) cout << "IN_CLOSE_NOWRITE ";
  if (e->mask & IN_CLOSE_WRITE) cout << "IN_CLOSE_WRITE ";
  if (e->mask & IN_CREATE) cout << "IN_CREATE ";
  if (e->mask & IN_DELETE) cout << "IN_DELETE ";
  if (e->mask & IN_DELETE_SELF) cout << "IN_DELETE_SELF ";
  if (e->mask & IN_IGNORED) cout << "IN_IGNORED ";
  if (e->mask & IN_ISDIR) cout << "IN_ISDIR ";
  if (e->mask & IN_MODIFY) cout << "IN_MODIFY ";
  if (e->mask & IN_MOVE_SELF) cout << "IN_MOVE_SELF ";
  if (e->mask & IN_MOVED_FROM) cout << "IN_MOVED_FROM ";
  if (e->mask & IN_MOVED_TO) cout << "IN_MOVED_TO ";
  if (e->mask & IN_OPEN) cout << "IN_OPEN ";
  if (e->mask & IN_Q_OVERFLOW) cout << "IN_Q_OVERFLOW ";
  if (e->mask & IN_UNMOUNT) cout << "IN_UNMOUNT ";
  cout << endl;

  if (e->len > 0) {
    cout << "     name = " << e->name << endl;
  }
}
