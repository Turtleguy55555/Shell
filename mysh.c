#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "linkedlist.c"
void interactive(FILE *fp, int mode) {
    char buffer[512];
    char* i = "mysh> ";
    if (mode == 1) {
        write(1, i, strlen(i));
    }
    
    while (fgets(buffer, 512, fp)) {
        if (mode == 0) {
            write(1, buffer, strlen(buffer));
        }
	int error = 0;
        buffer[strlen(buffer)-1] = '\0';
	if(strcmp(buffer,"exit") == 0) {
	    //freeList();
	    //break;
	}
        char *tokens = strdup(buffer);
        char *t1 = strdup(buffer);
        char *t2 = strdup(buffer);
        char *token = strtok(t1, " \t");
        int notBlanks = 0;
        int lock = 0;
        if (token != NULL && strcmp(token, ">") == 0) {
            char* message = "Redirection misformatted.\n";
            error = 1;
            lock = 1;
            write(2, message, strlen(message));
        }
        char *orig = strdup(buffer);
        char space = ' ';
        char *args[100];
        int counter = 0;
        int isRedir = 0;
        char* fileName;
        int isArrow = 0;
	char* save;
	char* b1;
	char* b2;
	char* proc1;
	char* proc2;
        int numArrows = 0;
        for (int i = 0; i < strlen(tokens); i++) {
            if (tokens[i] == '>') {
                isArrow = 1;
                numArrows++;
            }
        }
        char* misformat = "Redirection misformatted.\n";
        if (numArrows > 1 && isArrow == 1 && lock == 0) {
	    error = 1;
            write(2, misformat, strlen(misformat));
        }
        if (isArrow == 1 && lock == 0) {
            save = strtok(t2, ">");
            proc1 = strdup(save);
            save = strtok(NULL, ">");
            if (save == NULL) {
		error = 1;
                write(2, misformat, strlen(misformat));
            } else {
                b1 = strdup(proc1);
                proc2 = strdup(save);
		if(save != NULL) {
		    //free(save);
		}
                b2 = strdup(proc2);
                int i = 0;
                b1 = strtok(proc1, " \t");
                while (b1 != NULL) {
                   args[i] = b1;
                   b1 = strtok(NULL, " \t");
                   i++;
                }
                args[i] = NULL;
                if (b2 == NULL) {
                   error = 1;
                   write(2, misformat, strlen(misformat));
                } else {
                    b2 = strtok(proc2, " \t");
                    if (b2 == NULL) {
                        write(2, misformat, strlen(misformat));
                        error = 1;
                    } else if (strcmp(b2, ">") == 0) {
                        write(2, misformat, strlen(misformat));
                        error = 1;
                    } else {
                        char *token1 = strdup(b2);
                        b2 = strtok(NULL, " \t");
                        if (b2 == NULL) {
                            fileName = token1;
			    
                            isRedir = 1;
                        } else {
                            error = 1;
                            write(2, misformat, strlen(misformat));
                        }
                    }
                }
            }
        }
        int isAlias = 0;
        while (token != NULL && isArrow == 0) {
            if (strcmp(token, "unalias") == 0) {
                token = strtok(NULL, " ");
                isAlias = 1;
                if (token == NULL) {
                    write(2, "unalias: Incorrect number of arguments.\n", 40);
		    error = 1;
                } else {
                    char *inc = strdup(token);
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        write(2, "unalias: Incorrect number of arguments.\n",
                              40);
			error = 1;
                    } else {
                        delete(inc);
                    }
		    if(inc != NULL) {
			free(inc);
		    }
                }
                break;
            }
            if (strcmp(token, "alias") == 0 && counter == 0) {
                isAlias = 1;
                token = strtok(NULL, " ");
                if (token == NULL) {
                    list();
                    break;
                } else {
                    char* savior = strdup(token);
                    token = strtok(NULL, " ");
                    if (token == NULL) {
                        char* data = getData(savior);
                        if (data != NULL) {
                            write(1, savior, strlen(savior));
                            write(1, " ", 1);
                            write(1, data, strlen(data));
                            write(1, "\n", 1);
                        }
                        break;
                    } else {
                        char *leftover1 = strchr(tokens, space);
                        char *copy = strdup(leftover1);
                        char a[100];
                        for (int i = 0; i < 100; i++) {
                            a[i] = copy[i+1];
                            if (copy[i+1] == '\0') {
                                break;
                            }
                        }
                        char* c = strchr(a, space);
                        char* f = strdup(c);
                        char* l = strdup(f+1);

                        add(l, savior);
			if(l != NULL) {
			    //free(l);
			}
			if(f != NULL) {
			    //free(f);
			}
			if(copy != NULL) {
			    //free(copy);
			}
		   	
                        break;
                    }
                }
            } else {
                if (strcmp(token, " ") != 0 || strcmp(token, "\t") != 0) {
                    notBlanks = 1;
                    args[counter] = strdup(token);
                    token = strtok(NULL, " \t");
                    counter++;
                }
            }
        }
        if (isArrow == 0) {
            args[counter] = NULL;
        }
        pid_t pid = -1;
        if (error == 0 && isAlias == 0) {
            pid = fork();
        }
        if (pid >= 0 && error == 0) {
            if (pid == 0) {
                if (isRedir == 1) {
                    FILE *out = fopen(fileName, "w");
                    dup2(fileno(out), 1);
                    fclose(out);
                }
                char* data = getData(args[0]);
                if (data == NULL) {
                    int r = execv(args[0], args);
                    if (notBlanks == 1 && strlen(orig) != 0 &&
                        r < 0 && strcmp(orig, "exit") !=0 ) {
                        write(2, orig, strlen(orig));
                        write(2, ": Command not found.\n", 21);
                    }
		    int counters = 0;
		    while(args[counters] != NULL) {
			free(args[counters]);
			counters++;
		    }
                } else {
                    char* words[100];
                    char* dat = strdup(data);
                    char* tok = strtok(data, " ");
                    int index = 0;
                    while (tok != NULL) {
                        words[index] = tok;
                        tok = strtok(NULL, " ");
                        index++;
                    }
                    words[index] = NULL;
                    int r = execv(words[0], words);
                    if (r < 0 && strcmp(orig, "exit") != 0) {
                        write(2, dat, strlen(dat));
                        write(2, ": Command not found.\n", 21);
                    }
		    if(dat != NULL){
			free(dat);
		    }
		    int counterer = 0;
		    while(words[counterer] != NULL) {
			free(words[counterer]);
			counterer++;
	            }
		   
                }
                _exit(1);
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
        }
        if (mode == 1 && strcmp(orig, "exit") != 0) {
            write(1, i, strlen(i));
        }
	int escape = 0;
        if (strcmp(orig, "exit") == 0) {
            freeList();
	    escape = 1;
        }
	
	
	if(b1 != NULL) {
	    //free(b1);
	}
	if(b2 != NULL) {
	    //free(b2);
	}
	if(proc1 != NULL) {
	   // free(proc1);
	}
	if(proc2 != NULL) {
	    //free(proc2);
	}
	if(tokens != NULL) {
	    free(tokens);
	}
	if(orig != NULL) {
	    free(orig);
	}
	if(t1 != NULL) {
	    free(t1);
	}
	if(t2 != NULL) {
	    free(t2);
	}
	int counters = 0;
	while(args[counters] != NULL) {
	    //free(args[counters]);
	    counters++;
	}
	if(escape == 1) {
	    break;
	}
    }
    fclose(fp);
}


int main(int argc, char* argv[]) {
    if (argc == 1) {
        interactive(stdin, 1);
    } else if (argc == 2) {
        FILE *fp = fopen(argv[1], "r");
        if (fp == NULL) {
            char str[512];
            strcpy(str, "Error: Cannot open file ");
            strcat(str, argv[1]);
            strcat(str, ".\n");
            write(2, str, strlen(str));
            exit(1);
        }
        interactive(fp, 0);
    } else {
        char* str = "Usage: mysh [batch-file]\n";
        write(2, str, strlen(str));
        exit(1);
    }
    exit(0);
}

