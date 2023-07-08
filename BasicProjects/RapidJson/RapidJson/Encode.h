#pragma once
#include <stdlib.h>

static size_t calculateCharArraySize(char* array) {
    size_t count = 0;
    while (array[count] != '\0') {
        count++;
    }
    return count;
}

/*------ Base64 encoding Table ------*/
static const char MimeBase64[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};

/*------ Base64 Decoding Table ------*/
static int DecodeMimeBase64[256] = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 00-0F */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 10-1F */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,  /* 20-2F */
    52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,  /* 30-3F */
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,  /* 40-4F */
    15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,  /* 50-5F */
    -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,  /* 60-6F */
    41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,  /* 70-7F */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 80-8F */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 90-9F */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* A0-AF */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* B0-BF */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* C0-CF */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* D0-DF */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* E0-EF */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1   /* F0-FF */
};


/*
���ڵ� �⺻ ����
- 8 ��Ʈ x 3 ���� ==> 6 ��Ʈ x 4 ����

*/
// text : ���ڵ��� ���� �����Ͱ� ����ִ� ���ڿ� ������
// numBytes : ������ ũ��
// encodedText : ����� ���� ���� �ּ�
static int base64_encode_length(char* text, int numBytes)
{
    int   size;
    size = (4 * (numBytes / 3)) + (numBytes % 3 ? 4 : 0) + 1;
    return size;
}
static int base64_encode(char* text, int numBytes, char** encodedText)
{
    char input[3] = { 0,0,0 };
    char output[4] = { 0,0,0,0 };
    int   index, i, j, size;
    char* p, * plen;
    plen = text + numBytes - 1;
    size = (4 * (numBytes / 3)) + (numBytes % 3 ? 4 : 0) + 1;
    (*encodedText) = (char*)malloc(size);
    j = 0;
    for (i = 0, p = text; p <= plen; i++, p++) {
        index = i % 3;
        input[index] = *p;

        // 3 ���� ���ڸ� 24 ��Ʈ ���ۿ� ��´�.
        // ���� �̰��� 6 ��Ʈ * 4 ���·� ��ȯ�� ���̴�.
        // 3��° ���ڸ� �� �Է��߰ų� / ���ڿ� ���� �������� ��
        if (index == 2 || p == plen) {
            // ���� 6��Ʈ ä���
            // 0xFC : 1111 1100 
            // 1) input[0] �̶�� 8 bit �߿��� ���� 6�� bit ����, 6 �� ��Ʈ ���·� �����
            output[0] = ((input[0] & 0xFC) >> 2);
            
            // 0x3   : 0000 0011
            // 0xF0 : 1111 0000
            // 2) input[0] �� ���� 2 ��Ʈ + input[1]�� ���� 4��Ʈ == 6 bit
            output[1] = ((input[0] & 0x3) << 4) | ((input[1] & 0xF0) >> 4);
            
            // 0xF   :  0000 1111
            // 0xC0 :  1100 0000
            // 3) input[1] �� ���� 4��Ʈ + input[2] �� ���� 2��Ʈ == 6 bit
            output[2] = ((input[1] & 0xF) << 2) | ((input[2] & 0xC0) >> 6);
            
            // 0x3F : 0011 1111
            // 4) input[2] �� ���� 6 ��Ʈ ���� == 6 ��Ʈ
            output[3] = (input[2] & 0x3F);

            (*encodedText)[j++] = MimeBase64[output[0]];
            (*encodedText)[j++] = MimeBase64[output[1]];

            // index == 0 : ���� �ϳ��� �Է����� ���
            (*encodedText)[j++] = index == 0 ? '=' : MimeBase64[output[2]];
            
            // index < 2   : 3��° ���ڰ� �Էµ��� ���� ���
            (*encodedText)[j++] = index < 2 ? '=' : MimeBase64[output[3]];
            
            // �ٽ� reset
            input[0] = input[1] = input[2] = 0;
        }
    }
    (*encodedText)[j] = '\0';
    return size;
}

/*
���ڵ� ����
- ���ڵ��� ���ڿ��� Table �� �̿��Ͽ�, �ش� ���� 2�������� ǥ��
- 24 ��Ʈ ���ۿ� ���� ���� ��Ʈ���� 6��Ʈ�� 4�� �� �ֱ�
- 8��Ʈ�� �߶�, �� ���� ASCII �� �д´�.
- ���� '=' �� �е��Ǿ��ٸ� ����
- ASCII �� �ٽ� ���ڿ��� �о� ���
*/
static int base64_decode(char* encodedText, int numBytes, char* decodedText)
{
    const char* cp;
    int space_idx = 0, phase;
    int d, prev_d = 0;
    unsigned char c;
    space_idx = 0;
    phase = 0;
    for (cp = encodedText; *cp != '\0'; ++cp) {
        // ���ڿ� �ϳ��� �ش簪�� 2�������� ǥ��
        d = DecodeMimeBase64[(int)*cp];

        // 24��Ʈ ������ 6��Ʈ�� 4�� �� �ֱ�
        if (d != -1) {
            switch (phase) {
            case 0:
                ++phase;
                break;
            case 1:
                // 1) ���� 6��Ʈ �������� 2��Ʈ ���� + ���� 6��Ʈ �߿��� ���� 2��Ʈ �����ؼ� ���� 2��Ʈ�� ����� == 8 bit
                c = ((prev_d << 2) | ((d & 0x30) >> 4));
                if (space_idx < numBytes)
                    decodedText[space_idx++] = c;
                ++phase;
                break;
            case 2:
                // 2) ���� 6��Ʈ ��, ���� 4��Ʈ �����ؼ�, ���� 4��Ʈ�� �����
                //    + ���� 6��Ʈ �� ���� 4��Ʈ �̱� == bit
                c = (((prev_d & 0xf) << 4) | ((d & 0x3c) >> 2));
                if (space_idx < numBytes)
                    decodedText[space_idx++] = c;
                ++phase;
                break;
            case 3:
                // 3) ���� 6��Ʈ �� ���� 2��Ʈ + ���� 6��Ʈ
                c = (((prev_d & 0x03) << 6) | d);
                if (space_idx < numBytes)
                    decodedText[space_idx++] = c;
                phase = 0;
                break;
            }
            prev_d = d;
        }
    }
    return space_idx;
}
