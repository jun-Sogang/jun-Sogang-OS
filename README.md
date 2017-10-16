# jun-Sogang-OS


10/16 Project 1
- 하나도 모르겠다 ㅎㅎ 메뉴얼 대로 하라고 해도 안된다. 그래서 라이브러리와 thread 파일들을 해석하려 한다.
- 인터넷 검색 결과 threads/init.c 을 따라가다 threads/process.c 의 start_process에서 load 되기 전에 argument passing을 하면 된다고 한다. 아마 메모리에 적재되기 전에 parsing을 해야하는 것 같다. 일단 해본다.
- argument_stack을 다 만들었다.
```c
void argument_stack (char **wordSplit, int wordIndex, void **esp) {
  int i, wordLen, wordAlignControl = 0, alignedSize;
  for (i = wordIndex; i >= 0; --i) {
    wordLen = strlen(wordSplit[wordIndex]) + 1;
    wordAlignControl += wordLen;
    *esp -= wordLen;
    memcpy(*esp, wordSplit[wordIndex], wordLen + 1));
  }
  alignedSize = WORD_ALIGN(esp, wordAlignControl);
  memset(*esp, 0, alignedSize * sizeof(uint8_t));
  for (i = wordIndex; i >= 0; --i) {
    *esp -= CHAR_POINTER_SIZE;
    memcpy(*esp, &wordSplit[wordIndex], CHAR_POINTER_SIZE);
  }
  *esp -= CHAR_POINTER_SIZE;
  memcpy(*esp, wordSplit, CHAR_POINTER_SIZE);
  *esp -= INT_SIZE;
  //int num = wordIndex + 1;
  //memcpy(*esp, num, INT_SIZE);
  *((int *)*esp) = wordIndex + 1;
  *esp -= VOID_POINTER_SIZE;
  *((int *)*esp) = 0;
}
```
