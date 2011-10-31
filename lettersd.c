/* Daemon to solve letter sets for cntdn

   Networking information from:
   http://beej.us/guide/bgnet/output/html/multipage/clientserver.html

   James Stanley 2011 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "letters.h"

int ignore_invalid = 1;
int minletters = 0;

static struct option opts[] = {
    { "dictionary",     required_argument, NULL, 'd' },
    { "help",           no_argument,       NULL, 'h' },
    { "listen-addr",    required_argument, NULL, 'l' },
    { "max-clients",    required_argument, NULL, 'c' },
    { "port",           required_argument, NULL, 'p' },
    { NULL,             0,                 NULL, 0 }
};

void die(const char *fmt, ...) {
    fprintf(stderr, "Not dying. Segfault expected.\n");
}

static void usage(void) {
    printf(
    "lettersd - daemon for the countdown letters game solver\n"
    "Usage: lettersd [OPTIONS...]\n"
    "\n"
    "Options:\n"
    "  -d, --dictionary    Set the path to the dictionary file (default:\n"
    "                      " DEFAULT_DICT ")\n"
    "  -h, --help          Display this help\n"
    "  -l, --listen-addr   Set the address to listen on (default: localhost)\n"
    "  -c, --max-clients   Set the maximum number of concurrent clients\n"
    "                      (default: -1)\n"
    "  -p, --port          Set the port number to listen on (default: 17220)\n"
    "\n"
    "For the --max-clients option -1 means no limit.\n"
    "\n"
    "Report bugs to James Stanley <james@incoherency.co.uk>\n"
    );
}

/* open a socket on the given address and port, and return the associated fd */
static int start_listening(const char *addr, const char *port) {
    struct addrinfo hints, *servinfo, *p;
    int num_addrs;
    int yes = 1;
    int fd;
    int n;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    /* get a linked list of appropriate addresses that we can bind to */
    if((n = getaddrinfo(addr, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(n));
        return -1;
    }

    /* go through the linked list and bind to the first one we can */
    for(p = servinfo; p; p = p->ai_next) {
        num_addrs++;

        if((fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }

        if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            perror("setsockopt");
            /* we don't really care if this fails... */
        }

        if(bind(fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(fd);
            perror("bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    /* we reached the end of the servinfo list without successfully binding */
    if(!p) {
        fprintf(stderr, "failed to bind to any of %d addresses\n", num_addrs);
        return -1;
    }

    if(listen(fd, SOMAXCONN) == -1) {
        perror("listen");
        close(fd);
        return -1;
    }

    return fd;
}

/* callback for solve_letters to send a word to the client */
void send_word(const char *word, void *data) {
    int fd = *(int *)data;
    char nl = '\n';

    write(fd, word, strlen(word));
    write(fd, &nl, 1);
}

/* read letter sets from a client and solve them */
void *client_thread(void *arg) {
    int fd = *(int *)arg;
    char buf[1024];
    char *p;
    size_t gotbytes = 0;
    int n;
    char nl = '\n';

    while((n = read(fd, buf + gotbytes, 1024 - gotbytes)) > 0) {
        buf[gotbytes + n] = '\0';
        printf("read %s\n", buf + gotbytes);
        gotbytes += n;

        while((p = strchr(buf, 'X'))) {
            *p = '\0';

            solve_letters(buf, send_word, arg);

            write(fd, &nl, 1);

            memmove(buf, p + 1, gotbytes - (p + 1 - buf));

            gotbytes -= p + 1 - buf;
        }

        if(gotbytes >= 1024) {
            char *err = "!line too long\n";
            write(fd, err, strlen(err));
            break;
        }
    }

    if(n == -1) {
        perror("read");
    }

    close(fd);

    return NULL;
}

/* make a thread to handle a client */
void handle_client(int fd) {
    pthread_t tid;
    int *fd_ptr = malloc(sizeof(int));

    *fd_ptr = fd;

    if(pthread_create(&tid, NULL, client_thread, fd_ptr) != 0) {
        perror("pthread_create");
        exit(1);
    }
}

int main(int argc, char **argv) {
    char *dict = DEFAULT_DICT;
    char *listenaddr = "localhost";
    int maxclients = -1;
    char *port = "17220";
    socklen_t sin_size;
    struct sockaddr_storage their_addr;
    int fd;
    int c;

    opterr = 1;

    while((c = getopt_long(argc, argv, "d:hl:c:p:", opts, NULL)) != -1) {
        switch(c) {
            case 'd': dict = optarg;             break;
            case 'h': usage(); exit(0);          break;
            case 'l': listenaddr = optarg;       break;
            case 'c': maxclients = atoi(optarg); break;
            case 'p': port = optarg;             break;
            default: exit(1);
        }
    }

    load_dictionary(dict, 0);

    /* listen on the given address and port */
    if((fd = start_listening(listenaddr, port)) == -1)
        exit(1);

    /* repeatedly accept connections and deal with client letter sets */
    while(1) {
        sin_size = sizeof(their_addr);
        int clientfd = accept(fd, (struct sockaddr *)&their_addr, &sin_size);

        if(clientfd != -1) {
            handle_client(clientfd);
        } else {
            perror("accept");
            exit(1);
        }
    }
}
