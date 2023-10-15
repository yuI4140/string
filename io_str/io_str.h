bool createFile(const char *path);
bool appendToFile(const char *path, String *buffer);
String *extractLinef(String *path, int32_t line);
String *extractLine(String *content, int32_t line);
void readFile(const char *path, String *dest);
const char *extractFIleName(const char *file_path);
void mvFile(String *dest_path,String *src_path);
void cpFile(String *dest_path,String *src_path);
#ifdef IO_STRING_IMP
bool createFile(const char *path) {
  FILE *fp = fopen(path, "w");
  if (fp != NULL) {
    fclose(fp);
    return true;
  }
  return false;
}
bool appendToFile(const char *path, String *buffer) {
  FILE *fp = fopen(path, "a");
  if (fp != NULL) {
    fprintf(fp, "%s", (char *)c_str(buffer));
    fclose(fp);
    return true;
  }
  return false;
}
String *extractLinef(String *path, int32_t line) {
  FILE *extract = fopen(c_str(path), "r");
  mHandleIf(extract != NULL, "file_extract_null_check");
  String *buffer = allocStr(256);
  String *ret = allocStr(256);
  int32_t cline = 0;
  while ((fgets((char *)buffer->value, buffer->size, extract)) != NULL) {
    if (cline == line) {
      mvStr(ret, buffer);
    }
    ++cline;
  }
  fclose(extract);
  return ret;
}
String *extractLine(String *content, int32_t line) {
  mHandleIf((char *)content->value != NULL, "content_null_check");
  mHandleIf(line != 0, "line_min_check");
  String *buffer = allocStr(256);
  int32_t cline = 0;
  int32_t end = 0;
  int32_t prev_end = 0;
  int32_t start = -1;
  (void)prev_end;
  (void)start;
  mvStr(buffer, content);
  fiterate_str(content) {
    if (cline == line) {
      if (cline > 1) {
        strCut(buffer, prev_end, end);
      } else
        strCut(buffer, start, end);
      break;
    } else if (atStr(content, it) == '\n') {
      prev_end = end;
      end = it;
      ++cline;
      ++start;
    }
  }
  return buffer;
}
void readFile(const char *path, String *dest) {
  FILE *fp = fopen(path, "r");
  if (fp==NULL) {
   fprintf(stderr,"cannot open %s for read\n",path); 
   exit(0);
  }
  fseek(fp, 0, SEEK_END);
  size_t size_f = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  (void)fread((char *)dest->value, dest->size, size_f, fp);
  fclose(fp);
  dest->size = size_f;
}
void copyFromTo(char *src, char *dest, int32_t start, int32_t end) {
  assert(dest != NULL && src != NULL);
  memset(dest, 0, strlen(src));
  memmove(dest, src + start, end);
  size_t len = strlen(dest);
  dest[len + 1] = '\0';
}
const char *extractFIleName(const char *file_path) {
    const char *file_name = strrchr(file_path, '/'); 
    if (file_name == NULL) {
        file_name = file_path; 
    } else {
        file_name++;
    }
    return file_name;
}
void mvFile(String *dest_path,String *src_path){
    String *text_src=allocStr(1024*1024);
    pushCharpStr(dest_path,extractFIleName(c_str(src_path)));
    readFile(c_str(src_path),text_src);
    appendToFile(c_str(dest_path),text_src);
    remove(c_str(src_path));
}
void cpFile(String *dest_path,String *src_path){
    String *text_src=allocStr(1024*1024);
    pushCharpStr(dest_path,extractFIleName(c_str(src_path)));
    readFile(c_str(src_path),text_src);
    appendToFile(c_str(dest_path),text_src);
}

#endif // IO_STRING_IMP
