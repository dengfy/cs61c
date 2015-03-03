#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>

#include "beargit.h"
#include "util.h"

/* Implementation Notes:
 *
 * - Functions return 0 if successful, 1 if there is an error.
 * - All error conditions in the function description need to be implemented
 *   and written to stderr. We catch some additional errors for you in main.c.
 * - Output to stdout needs to be exactly as specified in the function description.
 * - Only edit this file (beargit.c)
 * - You are given the following helper functions:
 *   * fs_mkdir(dirname): create directory <dirname>
 *   * fs_rm(filename): delete file <filename>
 *   * fs_mv(src,dst): move file <src> to <dst>, overwriting <dst> if it exists
 *   * fs_cp(src,dst): copy file <src> to <dst>, overwriting <dst> if it exists
 *   * write_string_to_file(filename,str): write <str> to filename (overwriting contents)
 *   * read_string_from_file(filename,str,size): read a string of at most <size> (incl.
 *     NULL character) from file <filename> and store it into <str>. Note that <str>
 *     needs to be large enough to hold that string.
 *  - You NEED to test your code. The autograder we provide does not contain the
 *    full set of tests that we will run on your code. See "Step 5" in the homework spec.
 */

/* beargit init
 *
 * - Create .beargit directory
 * - Create empty .beargit/.index file
 * - Create .beargit/.prev file containing 0..0 commit id
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_init(void) {
  fs_mkdir(".beargit");

  FILE* findex = fopen(".beargit/.index", "w");
  fclose(findex);

  FILE* fbranches = fopen(".beargit/.branches", "w");
  fprintf(fbranches, "%s\n", "master");
  fclose(fbranches);
   
  write_string_to_file(".beargit/.prev", "0000000000000000000000000000000000000000");
  write_string_to_file(".beargit/.current_branch", "master");

  return 0;
}


/* beargit add <filename>
 * 
 * - Append filename to list in .beargit/.index if it isn't in there yet
 *
 * Possible errors (to stderr):
 * >> ERROR: File <filename> already added
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_add(const char* filename) {
  FILE* findex = fopen(".beargit/.index", "r");
  FILE *fnewindex = fopen(".beargit/.newindex", "w");

  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    if (strcmp(line, filename) == 0) {
      fprintf(stderr, "ERROR: File %s already added\n", filename);
      fclose(findex);
      fclose(fnewindex);
      fs_rm(".beargit/.newindex");
      return 3;
    }

    fprintf(fnewindex, "%s\n", line);
  }

  fprintf(fnewindex, "%s\n", filename);
  fclose(findex);
  fclose(fnewindex);

  fs_mv(".beargit/.newindex", ".beargit/.index");

  return 0;
}


/* beargit rm <filename>
 * 
 * See "Step 2" in the homework 1 spec.
 *
 */

int beargit_rm(const char* filename) {
  /* COMPLETE THE REST */

  FILE* findex = fopen(".beargit/.index", "r");
  FILE *fnewindex = fopen(".beargit/.newindex", "w");
  
  int tracked = 0;
  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    if (strcmp(line, filename) == 0) {
        tracked = 1;
	continue;
    }

    fprintf(fnewindex, "%s\n", line);
  }
  
  if (tracked == 0) {
      fprintf(stderr, "ERROR: File %s not tracked\n", filename);
  }

  fclose(findex);
  fclose(fnewindex);

  fs_mv(".beargit/.newindex", ".beargit/.index");

  return 0;
}

/* beargit commit -m <msg>
 *
 * See "Step 3" in the homework 1 spec.
 *
 */

const char* go_bears = "GO BEARS!";


char* concat(char *s1, char *s2) {
    int i = 0;
    int j = 0;
    while(*(s1 + i) != '\0') {
        i++;
    }

    while(*(s2 + j) != '\0') {
        j++;
    }

    int length = i + j + 1;
    char* result = malloc(length);
    if (result == NULL) {
        printf("\nMemory is FULL\n");
        exit(1);
    }
    int w;
    for (w = 0; w < i; w++) {
        *(result + w) = *(s1 + w);
    }
    for (w = i;w < i + j; w++) {
        *(result + w) = *(s2 + w - i);
    }

    *(result + length - 1) = '\0';
    return result;

}

char * Mystrstr (const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) {
        return NULL;
    }

    int i, j;
    for (i = 0; *(s1 + i) != '\0'; i++) {

        for (j = 0; *(s2 + j) != '\0'; j++) {
            if (*(s1 + i +j) != *(s2 + j)) {
                break;
            }
        }

        if (*(s2 + j) == '\0') {
            return (char*)(s1 + i);
        }

    }

    return NULL;
}

int is_commit_msg_ok(const char* msg) {
  /* COMPLETE THE REST */
  char * match;
  match = Mystrstr(msg, go_bears);
  if (match == NULL) {
      return 0;
  }else {
      return 1;
  }
}

void next_id(char* id, int n) {
    if (id[n - 1] == '6') {
        id[n - 1] = '1';
    }
    else if (id[n - 1] == '1') {
        id[n - 1] = 'c';
    }
    else if (id[n - 1] == 'c') {
        id[n - 1] = '6';
        next_id(id, n - 1);
    }
}

void next_commit_id_hw1(char* commit_id) {
  /* COMPLETE THE REST */
	int length = 0;
	while(commit_id[length] != '\0') {
		length++;
	}

	if (commit_id[0] == '0') {
		int i;
    	for (i = 0; i < length; i++) {
            commit_id[i] = '6';
    	}
	}else{
		next_id(commit_id, length);
	} 
}

int beargit_commit_hw1(const char* msg) {
  if (!is_commit_msg_ok(msg)) {
    fprintf(stderr, "ERROR: Message must contain \"%s\"\n", go_bears);
    return 1;
  }

  char commit_id[COMMIT_ID_SIZE];
  read_string_from_file(".beargit/.prev", commit_id, COMMIT_ID_SIZE);
  next_commit_id(commit_id);

  /* COMPLETE THE REST */
  char* dirname = concat(".beargit/", commit_id);
  fs_mkdir(dirname);
  char* fcp_index = concat(dirname, "/.index");
  char* fcp_prev = concat(dirname, "/.prev");
  fs_cp(".beargit/.index", fcp_index);
  fs_cp(".beargit/.prev", fcp_prev);  
  free(fcp_index);
  free(fcp_prev);  


  FILE* findex = fopen(".beargit/.index", "r");
  char line[FILENAME_SIZE];

  while(fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
	char* fcp_tmp = concat(dirname, "/");
	char* fcp_file = concat(fcp_tmp, line);
	fs_cp(line, fcp_file);
	free(fcp_tmp);
	free(fcp_file);
  }
  fclose(findex);
  
  char* msg_filename = concat(dirname, "/.msg");
  FILE* msg_file = fopen(msg_filename, "w");
  fclose(msg_file);
  write_string_to_file(msg_filename, msg);
  free(msg_filename);  
 
  FILE* fnewprev = fopen(".beargit/.newprev", "w");
  fclose(fnewprev);
  write_string_to_file(".beargit/.newprev", commit_id);
  fs_mv(".beargit/.newprev", ".beargit/.prev");
  free(dirname);
  return 0;
}

/* beargit status
 *
 * See "Step 1" in the homework 1 spec.
 *
 */

int beargit_status() {
  /* COMPLETE THE REST */

  FILE* findex = fopen(".beargit/.index", "r");
  char line[FILENAME_SIZE];
  int count = 0;

  fprintf(stdout, "%s\n\n", "Tracked files:");
  
  while(fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    fprintf(stdout, "  %s\n", line);
    count = count + 1;
  }

  fprintf(stdout, "\n%d %s\n", count, "files total");
  fclose(findex);
  return 0;
}

/* beargit log
 *
 * See "Step 4" in the homework 1 spec.
 *
 */
void print_log(char* commit_id) {
    if (commit_id[0] == '0') {
        return;
    }

    fprintf(stdout, "commit %s\n", commit_id);
    char* dirname = concat(".beargit/", commit_id);
    char* msg_filename = concat(dirname, "/.msg");
    char* prev_filename = concat(dirname, "/.prev");
    char msg[MSG_SIZE];
    char last_commit_id[COMMIT_ID_SIZE];

    read_string_from_file(msg_filename, msg, MSG_SIZE);
    read_string_from_file(prev_filename, last_commit_id, COMMIT_ID_SIZE);
    fprintf(stdout, "    %s\n", msg);
    print_log(last_commit_id);

    free(dirname);
    free(msg_filename);
    free(prev_filename);

}



int beargit_log() {
  /* COMPLETE THE REST */
  char commit_id[COMMIT_ID_SIZE];
  read_string_from_file(".beargit/.prev", commit_id, COMMIT_ID_SIZE);
  if (commit_id[0] == '0') {
	fprintf(stderr, "ERROR: There are no commits!");
	return 1;
  }else{
	fprintf(stdout, "\n");

	print_log(commit_id);

	fprintf(stdout, "\n");
	return 0;
  }
}


// ---------------------------------------
// HOMEWORK 2 
// ---------------------------------------

// This adds a check to beargit_commit that ensures we are on the HEAD of a branch.
int beargit_commit(const char* msg) {
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", current_branch, BRANCHNAME_SIZE);

  if (strlen(current_branch) == 0) {
    fprintf(stderr, "ERROR: Need to be on HEAD of a branch to commit\n");
    return 1;
  }

  return beargit_commit_hw1(msg);
}

const char* digits = "61c";

void next_commit_id(char* commit_id) {
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", current_branch, BRANCHNAME_SIZE);

  // The first COMMIT_ID_BRANCH_BYTES=10 characters of the commit ID will
  // be used to encode the current branch number. This is necessary to avoid
  // duplicate IDs in different branches, as they can have the same pre-
  // decessor (so next_commit_id has to depend on something else).
  int n = get_branch_number(current_branch);
  for (int i = 0; i < COMMIT_ID_BRANCH_BYTES; i++) {
    // This translates the branch number into base 3 and substitutes 0 for
    // 6, 1 for 1 and c for 2.
    commit_id[i] = digits[n%3];
    n /= 3;
  }

  // Use next_commit_id to fill in the rest of the commit ID.
  next_commit_id_hw1(commit_id + COMMIT_ID_BRANCH_BYTES);
}


// This helper function returns the branch number for a specific branch, or
// returns -1 if the branch does not exist.
int get_branch_number(const char* branch_name) {
  FILE* fbranches = fopen(".beargit/.branches", "r");

  int branch_index = -1;
  int counter = 0;
  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), fbranches)) {
    strtok(line, "\n");
    if (strcmp(line, branch_name) == 0) {
      branch_index = counter;
    }
    counter++;
  }

  fclose(fbranches);

  return branch_index;
}

/* beargit branch
 *
 * See "Step 6" in the homework 1 spec.
 *
 */

int beargit_branch() {
  /* COMPLETE THE REST */

  return 0;
}

/* beargit checkout
 *
 * See "Step 7" in the homework 1 spec.
 *
 */

int checkout_commit(const char* commit_id) {
  /* COMPLETE THE REST */
  return 0;
}

int is_it_a_commit_id(const char* commit_id) {
  /* COMPLETE THE REST */
  return 1;
}

int beargit_checkout(const char* arg, int new_branch) {
  // Get the current branch
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", "current_branch", BRANCHNAME_SIZE);

  // If not detached, update the current branch by storing the current HEAD into that branch's file...
  // Even if we cancel later, this is still ok.
  if (strlen(current_branch)) {
    char current_branch_file[BRANCHNAME_SIZE+50];
    sprintf(current_branch_file, ".beargit/.branch_%s", current_branch);
    fs_cp(".beargit/.prev", current_branch_file);
  }

  // Check whether the argument is a commit ID. If yes, we just stay in detached mode
  // without actually having to change into any other branch.
  if (is_it_a_commit_id(arg)) {
    char commit_dir[FILENAME_SIZE] = ".beargit/";
    strcat(commit_dir, arg);
    if (!fs_check_dir_exists(commit_dir)) {
      fprintf(stderr, "ERROR: Commit %s does not exist\n", arg);
      return 1;
    }

    // Set the current branch to none (i.e., detached).
    write_string_to_file(".beargit/.current_branch", "");

    return checkout_commit(arg);
  }

  // Just a better name, since we now know the argument is a branch name.
  const char* branch_name = arg;

  // Read branches file (giving us the HEAD commit id for that branch).
  int branch_exists = (get_branch_number(branch_name) >= 0);

  // Check for errors.
  if (!(!branch_exists || !new_branch)) {
    fprintf(stderr, "ERROR: A branch named %s already exists\n", branch_name);
    return 1;
  } else if (!branch_exists && new_branch) {
    fprintf(stderr, "ERROR: No branch %s exists\n", branch_name);
    return 1;
  }

  // File for the branch we are changing into.
  char* branch_file = ".beargit/.branch_"; 
  strcat(branch_file, branch_name);

  // Update the branch file if new branch is created (now it can't go wrong anymore)
  if (new_branch) {
    FILE* fbranches = fopen(".beargit/.branches", "a");
    fprintf(fbranches, "%s\n", branch_name);
    fclose(fbranches);
    fs_cp(".beargit/.prev", branch_file); 
  }

  write_string_to_file(".beargit/.current_branch", branch_name);

  // Read the head commit ID of this branch.
  char branch_head_commit_id[COMMIT_ID_SIZE];
  read_string_from_file(branch_file, branch_head_commit_id, COMMIT_ID_SIZE);

  // Check out the actual commit.
  return checkout_commit(branch_head_commit_id);
}
