#include "file_manager.h"

// Class file_io
int _show_txt(file* f) {
  for (size_t i = 0;i < f->txt->num_of_lines;i++) {
    puts(f->txt->lines[i]);
  }
  return SUCCESS;
}

int _load_txt(file* f, char* filename) {

  // Initializing text
  // TODO Code refactoring
  f->txt = (text*) calloc(1, sizeof(text));
  f->txt->lines = (char**) malloc(sizeof(char*));
  f->txt->num_of_lines = 0;
  f->txt->initialized = true;

  // Initializing file
  f->current_file = fopen(filename, "r+");
  f->filename = filename;
  
  if (!f->current_file) {
    _destroy_txt(f->txt);
    return ERROR;
  }

  f->initialized = true;
  // end of Initialization

  char* line_buffer = malloc(256);
  if (!line_buffer) {
    _destroy_txt(f->txt);
    _destroy_file(f);
    return ERROR;
  }

  size_t char_num_dump = 0;
  size_t* l_num = &f->txt->num_of_lines;
  size_t line_size = 0;

  while((line_size = getline(&line_buffer,
        &char_num_dump, f->current_file)) != -1) {

    f->txt->lines[*l_num] = calloc(sizeof(char), line_size + 1);
    memcpy(f->txt->lines[*l_num], line_buffer, line_size);

    if (f->txt->lines[*l_num][line_size-1] == '\n')  
      f->txt->lines[*l_num][line_size-1] = '\0';
      
    *l_num = *l_num + 1;
    f->txt->lines = realloc(f->txt->lines, sizeof(char*)*(*l_num + 1));
  }

  free(line_buffer);
  return SUCCESS;
}

int _destroy_txt(text* txt) {
  for (size_t i = 0; i < txt->num_of_lines; i++) {
    free(txt->lines[i]);
  }
  free(txt->lines);
  free(txt);

  return SUCCESS;
}

int _destroy_file(file* f) {
  if (f) {
    if (f->txt) {
      _destroy_txt(f->txt);
    }
    fclose(f->current_file);
    free(f);
  }
  return SUCCESS;
}

int _write_file(file* f) {
  // For now, the original file will not be replace
  FILE* out_file;

  char* out_file_name = calloc(256, sizeof(char));
  if (!out_file_name){
    return ERROR;
  }

  strcpy(out_file_name, strcat(f->filename, ".out"));

  out_file = fopen(out_file_name, "w+");
  if (!out_file) {
    return ERROR;
  }

  for (size_t i = 0; i < f->txt->num_of_lines; i++) {
    fprintf(out_file, "%s\n", f->txt->lines[i]);
  }

  fclose(out_file);
  free(out_file_name);

  return SUCCESS;
}