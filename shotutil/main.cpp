#include <sys/inotify.h>
#include <unistd.h>
#include <errno.h>
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


int main(int argc, const char *argv[]) {
  if (argc < 2) {
    cout << "need app argument" << endl;
    return -1;
  }

  string filename = argv[1];
  string currentDir = getCurrentDir();
  ostringstream buf;
  buf << currentDir << '/' << filename;
  string absname = buf.str();

  if (!existsFile(absname)) {
    cout << "not exist app filename: " << filename << endl;
    return -1;
  }

  // parse options
  cout << "argc: " << argc << '\n';
  for (int i = 0; i < argc; ++i) {
    cout << argv[i] << endl;
  }

  cout << "monitor file: " << absname << endl;

  // inotify init
  int notifyId = inotify_init();
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
  int dirId = inotify_add_watch(notifyId, currentDir.c_str(), IN_MOVED_TO | IN_DELETE);
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

  int numRead;
  size_t NAME_MAX = 4096;
  size_t BUF_SIZE = 10 * (sizeof(struct inotify_event) + NAME_MAX + 1);
  char eventsBuf[BUF_SIZE];
  char *cursor;
  struct inotify_event* event;
  int error;

  // epoll event loop
  // TODO: make epoll like in shot

  for (;;) { // read events forever
    // inotofy add watch

    numRead = read(notifyId, eventsBuf, BUF_SIZE);
    if (numRead == 0) {
      cout << "read() from notifyId returned 0!" << endl;
      return -1;
    }
    if (numRead == -1) {
      cout << "read error" << endl;
    }

    cout << "read " << numRead << " bytes from notifyId" << endl;

    // process all of the events in buffer
    for (cursor = eventsBuf; cursor < eventsBuf + numRead; ) {
      event = (struct inotify_event*)(cursor);
      cursor += sizeof(struct inotify_event) + event->len;
      displayInotifyEvent(event);
      if (event->name == filename) {
        if (event->mask & IN_DELETE) {
          cout << "kill " << filename << endl;
        }
        if (event->mask & IN_MOVED_TO) {
          cout << "start " << filename << endl;
        }
      }
      /* if (event->mask & IN_MODIFY) { */
      /*   cout << "restart " << filename << endl; */
      /* } */
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


void displayInotifyEvent(struct inotify_event* e) {
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
