int lengthOfLastWord(char* s) {
    int i = 0;
    int cur = 0;
    int last = 0;
    while (s[i] != '\0') {
        if (s[i] == ' ') {
            if (cur > 0) {
                last = cur;
            }
            cur = 0;
        } else {
            cur++;
        }
        i++;
    }
    if (cur > 0) {
        return cur;
    } else {
        return last;
    }
}