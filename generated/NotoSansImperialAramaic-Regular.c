#ifndef __STRICT_ANSI__
#if defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__)
#if !defined(__ICC)
#define HAVE_INCBIN
#endif
#endif
#endif

#ifdef HAVE_INCBIN
const int fz_font_NotoSansImperialAramaic_Regular_ttf_size = 5632;
asm(".section .rodata");
asm(".global fz_font_NotoSansImperialAramaic_Regular_ttf");
asm(".type fz_font_NotoSansImperialAramaic_Regular_ttf STT_OBJECT");
asm(".size fz_font_NotoSansImperialAramaic_Regular_ttf, 5632");
asm(".balign 64");
asm("fz_font_NotoSansImperialAramaic_Regular_ttf:");
asm(".incbin \"resources/fonts/noto/NotoSansImperialAramaic-Regular.ttf\"");
#else
const int fz_font_NotoSansImperialAramaic_Regular_ttf_size = 5632;
const char fz_font_NotoSansImperialAramaic_Regular_ttf[] = {
0,1,0,0,0,11,0,128,0,3,0,48,79,83,47,50,122,231,113,147,0,0,1,56,0,0,0,96,
99,109,97,112,0,221,33,93,0,0,2,36,0,0,0,184,103,97,115,112,0,22,0,11,0,0,
21,240,0,0,0,16,103,108,121,102,25,71,250,116,0,0,3,36,0,0,12,110,104,101,
97,100,4,89,110,163,0,0,0,188,0,0,0,54,104,104,101,97,15,87,5,51,0,0,0,244,
0,0,0,36,104,109,116,120,175,254,15,54,0,0,1,152,0,0,0,140,108,111,99,97,
46,199,50,57,0,0,2,220,0,0,0,72,109,97,120,112,0,39,0,56,0,0,1,24,0,0,0,32,
110,97,109,101,148,92,189,215,0,0,15,148,0,0,6,58,112,111,115,116,255,105,
0,102,0,0,21,208,0,0,0,32,0,1,0,0,0,1,7,174,7,43,217,11,95,15,60,245,0,11,
8,0,0,0,0,0,204,73,81,10,0,0,0,0,210,40,203,239,255,196,253,202,7,4,7,52,
0,0,0,9,0,2,0,0,0,0,0,0,0,1,0,0,8,141,253,168,0,0,7,84,255,196,0,19,7,4,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35,0,1,0,0,0,35,0,55,0,3,0,0,0,0,0,1,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,5,45,1,144,0,5,0,0,5,154,5,51,0,0,1,31,5,
154,5,51,0,0,3,209,0,102,2,0,0,0,2,11,5,2,4,5,4,2,2,4,0,0,0,0,6,0,0,0,0,0,
0,0,0,0,0,0,71,79,79,71,0,64,0,0,255,255,8,141,253,168,0,0,8,141,2,88,0,0,
0,1,0,0,0,0,4,0,5,154,0,0,0,32,0,0,4,0,0,102,0,0,0,0,4,20,0,0,2,20,0,0,6,
161,0,100,5,202,0,100,6,161,0,40,5,17,0,40,6,152,0,100,3,107,0,60,2,231,0,
230,5,241,0,40,5,184,0,160,4,246,0,100,4,151,0,10,4,199,0,160,5,174,0,40,
3,125,0,130,5,242,0,80,4,18,0,100,4,130,0,100,4,210,0,200,5,13,0,200,5,94,
0,100,7,84,0,160,5,23,255,196,5,126,0,120,3,23,0,200,4,203,0,200,6,205,0,
200,6,22,0,150,6,138,0,150,6,82,0,216,6,7,0,150,6,82,0,216,0,0,0,2,0,3,0,
1,0,0,0,20,0,3,0,10,0,0,0,84,0,4,0,64,0,0,0,12,0,8,0,2,0,4,0,0,0,13,0,32,
0,160,254,255,255,255,0,0,0,0,0,13,0,32,0,160,254,255,255,255,0,1,255,245,
255,227,255,99,1,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,12,0,0,0,0,0,100,0,0,0,0,
0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,13,0,0,0,13,0,0,0,2,0,0,0,32,0,0,0,
32,0,0,0,3,0,0,0,160,0,0,0,160,0,0,0,3,0,0,254,255,0,0,254,255,0,0,0,1,0,
1,8,64,0,1,8,85,0,0,0,4,0,1,8,87,0,1,8,95,0,0,0,26,0,0,0,20,0,20,0,20,0,20,
0,72,0,126,0,171,0,212,1,8,1,46,1,77,1,136,1,197,1,228,2,30,2,79,2,137,2,
187,2,249,3,26,3,68,3,111,3,171,3,224,4,19,4,77,4,134,4,161,4,206,5,11,5,
47,5,104,5,168,5,223,6,55,0,2,0,102,0,0,3,154,5,182,0,3,0,7,0,0,51,17,33,
17,37,33,17,33,102,3,52,253,47,2,110,253,146,5,182,250,74,98,4,242,0,1,0,
100,255,206,6,61,5,214,0,26,0,0,1,6,2,17,20,23,7,2,0,39,22,21,20,2,7,39,54,
18,53,52,39,55,0,1,54,18,55,6,61,158,174,27,129,177,254,175,150,7,147,126,
133,102,126,64,140,1,141,1,29,12,207,160,5,95,235,253,228,254,255,217,95,
81,1,103,2,52,214,61,35,150,254,189,119,119,87,1,44,167,218,111,93,254,23,
253,210,248,2,56,219,0,0,1,0,100,255,206,5,42,5,202,0,28,0,0,1,22,21,16,2,
2,4,5,39,54,36,54,54,18,19,6,4,35,32,3,55,30,2,51,50,54,54,55,5,32,10,122,
255,254,87,254,210,59,233,1,71,219,131,64,8,123,254,204,151,254,142,89,170,
29,78,109,99,77,233,201,43,5,202,122,164,254,201,254,128,254,238,207,70,161,
58,142,167,208,1,6,1,3,107,117,1,191,52,127,144,63,78,133,70,0,0,1,0,40,255,
206,6,131,5,202,0,21,0,0,19,54,36,0,18,63,3,6,21,16,18,22,22,23,7,36,0,3,
0,5,40,125,1,12,1,0,222,45,6,7,178,6,65,125,198,138,59,254,210,254,182,14,
254,132,254,77,1,155,60,215,1,22,1,59,104,24,23,52,78,170,254,247,254,182,
247,206,76,160,138,2,50,1,145,253,221,235,0,1,0,40,255,206,4,113,5,204,0,
21,0,0,1,2,3,39,52,18,19,6,6,35,32,3,55,30,2,51,50,54,54,55,55,4,113,102,
70,168,65,42,96,236,119,254,142,43,170,14,65,98,82,92,183,163,60,13,5,162,
254,117,251,183,50,147,2,227,1,0,78,88,1,178,52,114,142,65,69,132,101,41,
0,1,0,100,255,206,5,248,5,204,0,28,0,0,37,54,18,53,52,38,38,35,34,7,6,2,7,
39,54,18,55,5,39,37,54,36,51,50,22,18,21,16,1,4,14,142,168,113,128,30,62,
215,14,192,160,137,125,166,26,254,58,60,1,203,176,1,63,57,67,188,162,254,
159,57,220,1,178,158,51,226,173,93,243,253,244,235,107,194,1,171,192,201,
138,208,80,115,219,254,221,58,254,63,253,251,0,1,0,60,255,206,2,203,5,201,
0,21,0,0,19,54,51,50,18,17,21,20,2,7,39,54,18,53,52,46,2,35,34,7,7,60,111,
120,210,214,35,30,178,26,37,31,65,107,77,83,93,4,5,170,31,254,228,254,227,
135,218,254,47,144,50,145,1,170,200,216,174,103,52,19,1,0,0,1,0,230,255,206,
1,227,5,202,0,13,0,0,1,16,2,7,39,54,18,17,52,2,39,23,22,18,1,227,41,35,177,
33,40,11,4,179,5,11,4,2,254,205,253,189,190,50,198,2,57,1,10,134,1,25,34,
20,30,254,243,0,0,1,0,40,255,206,5,81,6,28,0,34,0,0,55,54,18,18,53,39,38,
39,55,22,4,51,50,36,55,22,17,16,3,39,54,18,53,52,39,6,35,34,39,23,21,20,2,
2,7,40,112,163,88,6,136,72,100,80,1,15,159,99,1,15,77,115,212,154,88,98,32,
187,200,95,85,1,105,183,126,75,112,1,73,1,175,227,101,67,98,124,98,109,76,
54,171,254,191,254,24,254,124,95,174,1,135,177,198,113,75,19,47,60,218,254,
58,254,159,129,0,1,0,160,255,212,5,24,5,203,0,37,0,0,1,55,54,54,51,50,22,
21,16,2,4,35,34,38,2,53,52,18,55,23,6,2,21,20,18,22,51,50,54,18,53,52,38,
38,35,34,6,7,2,225,50,119,197,50,81,70,145,254,239,195,156,238,137,180,203,
104,164,143,93,163,110,126,186,106,18,30,34,25,137,100,4,25,19,47,53,129,
137,254,203,254,84,209,181,1,93,211,252,1,114,164,135,130,254,210,224,166,
254,245,138,165,1,81,219,58,64,28,51,37,0,1,0,100,2,246,4,146,5,235,0,13,
0,0,1,7,38,0,39,6,7,39,54,18,55,23,22,22,4,146,59,172,254,248,79,192,205,
99,140,252,78,146,70,216,3,150,160,78,1,2,174,217,164,149,114,1,4,105,65,
188,251,0,0,1,0,10,254,32,3,247,5,204,0,34,0,0,19,54,36,18,53,17,52,55,6,
35,34,38,53,52,55,23,6,21,20,22,51,50,54,55,54,55,23,6,2,21,17,16,2,4,5,10,
247,1,54,146,12,125,151,140,154,17,176,13,73,67,78,188,66,14,20,174,53,69,
186,254,137,254,228,254,196,46,181,1,36,240,1,84,123,114,107,190,178,87,114,
50,95,63,90,106,185,136,42,43,42,105,254,143,222,255,0,254,210,254,126,231,
51,0,0,1,0,160,255,128,4,180,5,200,0,28,0,0,5,54,55,6,4,35,34,38,17,16,1,
23,2,2,21,20,22,51,50,55,55,54,51,50,22,21,20,6,7,3,168,72,10,104,254,238,
91,198,191,1,3,165,121,123,101,128,138,255,36,84,26,48,48,49,45,128,130,80,
55,70,253,1,0,1,202,2,44,58,255,0,254,29,207,205,149,111,17,41,55,47,58,167,
91,0,0,1,0,40,254,252,5,14,6,120,0,33,0,0,1,6,7,22,51,50,55,22,21,20,2,3,
39,18,18,53,52,39,6,35,34,39,2,7,39,54,55,38,39,55,22,23,54,55,3,53,74,117,
112,127,217,159,49,245,228,137,196,234,8,132,107,174,149,174,203,127,186,
161,143,85,145,55,114,116,62,6,54,212,216,38,27,73,168,197,253,153,254,154,
97,1,61,2,59,156,56,34,12,56,254,236,175,115,164,252,95,146,99,118,78,211,
206,0,0,1,0,130,254,252,2,221,5,208,0,32,0,0,23,54,18,53,52,38,35,34,7,6,
35,34,38,53,52,18,55,23,6,6,21,20,51,50,55,54,51,50,22,21,20,2,7,223,134,
196,21,20,28,65,65,35,93,96,116,84,142,80,82,66,41,92,91,30,48,55,195,170,
174,183,1,235,128,80,50,10,10,133,123,116,1,18,104,98,117,206,73,91,10,10,
92,96,221,253,239,245,0,1,0,80,254,252,5,82,5,208,0,32,0,0,1,6,6,7,54,54,
55,23,23,16,2,2,4,5,39,54,36,54,54,18,55,6,4,7,39,54,55,6,5,39,54,36,55,3,
125,23,64,57,156,243,62,148,4,120,254,254,91,254,203,59,236,1,77,221,131,
60,2,116,254,135,104,61,102,68,249,254,200,53,194,1,127,94,5,87,106,206,107,
56,134,69,63,188,254,205,254,141,254,246,200,72,161,59,138,162,201,1,2,251,
84,141,3,145,152,226,142,95,152,66,174,68,0,1,0,100,3,7,3,174,5,217,0,17,
0,0,1,6,21,20,22,51,50,18,55,23,2,2,35,34,38,53,52,55,1,80,56,86,86,126,173,
19,172,31,252,212,160,187,75,5,141,141,109,123,108,1,40,243,19,254,178,254,
161,218,183,139,182,0,0,1,0,100,255,206,3,226,5,212,0,19,0,0,1,22,4,23,16,
2,0,7,39,36,0,19,38,38,39,6,7,39,54,54,1,224,159,1,32,67,160,254,188,244,
104,1,62,1,66,12,52,172,86,154,155,95,68,167,5,212,41,156,79,254,233,254,
74,254,134,171,136,220,2,3,1,58,43,90,32,148,92,120,45,136,0,1,0,200,254,
252,4,110,5,208,0,25,0,0,23,54,18,53,52,2,39,55,30,2,51,50,54,55,23,6,6,35,
34,39,22,21,20,2,7,229,57,68,78,76,149,73,105,115,72,101,122,26,171,26,215,
163,123,84,11,75,61,211,176,1,242,255,186,1,96,146,86,167,156,73,140,139,
55,179,210,42,91,173,251,253,248,194,0,2,0,200,254,252,4,109,5,208,0,22,0,
35,0,0,23,18,18,53,52,2,39,55,22,23,54,54,51,50,22,22,21,20,2,0,7,2,3,19,
22,21,21,62,2,53,52,38,35,34,6,229,74,51,88,66,156,40,32,113,242,85,95,108,
62,156,254,233,170,21,109,76,60,117,193,109,51,70,53,204,211,1,24,1,82,210,
208,1,197,124,86,74,90,72,87,54,114,94,123,254,224,254,246,95,254,148,254,
167,5,147,246,255,20,72,206,214,80,52,48,84,0,1,0,100,253,202,4,190,5,204,
0,28,0,0,1,54,18,19,18,55,6,6,35,32,3,55,30,2,51,50,54,54,55,55,23,6,3,10,
2,6,7,2,55,161,151,19,18,31,90,230,114,254,142,43,170,14,65,98,82,89,180,
158,61,23,174,104,18,12,54,90,135,94,254,44,202,1,186,1,118,1,116,207,71,
82,1,178,52,114,142,65,66,124,97,56,42,205,254,21,254,187,254,148,254,229,
241,99,0,1,0,160,255,232,7,4,5,202,0,24,0,0,1,2,3,18,51,50,54,0,55,23,2,0,
4,35,34,38,2,2,39,55,22,18,23,18,19,3,253,122,251,175,118,60,242,1,15,115,
167,144,254,188,254,227,133,91,207,222,201,29,180,24,135,71,176,112,4,189,
254,145,254,190,254,129,234,1,126,212,61,254,241,254,75,224,221,1,186,2,94,
186,51,138,254,67,186,1,10,1,43,0,0,1,255,196,254,252,4,119,5,213,0,32,0,
0,1,18,18,53,52,38,35,34,4,7,23,16,2,3,39,18,18,17,52,38,39,55,22,23,36,51,
50,22,22,21,20,2,7,2,149,124,178,52,24,44,254,239,166,2,160,152,154,156,130,
30,23,163,32,19,1,127,170,42,89,72,191,122,1,46,1,5,2,22,126,25,76,139,105,
100,254,211,253,139,254,202,75,1,91,2,35,1,10,142,242,68,59,93,133,233,76,
137,48,154,253,177,247,0,0,1,0,120,255,206,5,6,5,210,0,31,0,0,37,54,18,53,
52,39,38,39,55,22,23,38,3,55,22,23,54,55,23,6,6,7,36,55,23,4,5,22,21,20,2,
7,1,233,28,33,26,226,178,20,160,184,64,124,165,95,65,121,197,104,99,171,42,
1,2,234,63,254,239,254,205,24,43,32,2,73,1,22,163,128,132,10,31,164,30,10,
229,1,6,58,206,207,244,161,135,77,222,106,16,66,156,90,4,134,134,134,254,
185,95,0,0,1,0,200,255,236,2,79,5,203,0,11,0,0,1,18,18,17,7,39,55,53,52,2,
2,39,1,121,94,120,3,178,1,47,99,65,5,203,254,194,252,250,254,213,112,20,40,
40,153,1,199,2,29,221,0,2,0,200,255,236,4,3,5,203,0,11,0,21,0,0,1,18,18,17,
7,39,55,53,52,2,2,39,37,21,16,2,3,39,18,18,17,53,1,121,94,120,3,178,1,47,
99,65,3,59,98,89,166,77,95,5,203,254,194,252,250,254,213,112,20,40,40,153,
1,199,2,29,221,33,112,254,144,253,30,254,227,45,1,8,3,4,1,89,44,0,0,3,0,200,
255,236,6,5,5,203,0,11,0,21,0,29,0,0,1,18,18,17,7,39,55,53,52,2,2,39,37,21,
16,2,3,39,18,18,17,53,37,2,2,3,39,18,18,19,1,121,94,120,3,178,1,47,99,65,
3,59,98,89,166,77,95,2,183,48,234,141,168,121,227,51,5,203,254,194,252,250,
254,213,112,20,40,40,153,1,199,2,29,221,33,112,254,144,253,30,254,227,45,
1,8,3,4,1,89,44,33,254,143,252,186,254,216,45,1,7,3,40,1,98,0,1,0,150,1,3,
5,128,3,243,0,18,0,0,19,36,33,50,30,3,21,16,3,39,18,55,38,36,35,32,7,150,
1,2,1,72,87,202,207,142,34,158,156,114,17,111,254,212,127,254,225,230,3,200,
43,16,32,35,23,25,254,232,254,171,79,1,1,195,23,33,42,0,0,1,0,150,0,78,5,
244,4,227,0,34,0,0,19,54,51,50,4,23,22,21,20,7,22,23,22,21,20,3,39,54,55,
38,36,35,34,7,39,54,51,23,54,55,38,36,35,34,7,150,162,179,165,1,106,112,33,
64,249,136,40,123,164,89,15,111,254,212,127,128,147,17,162,179,64,55,9,111,
254,212,127,128,147,4,213,14,47,37,11,36,181,201,27,43,13,35,245,254,215,
64,216,172,23,33,11,162,14,1,160,120,23,33,11,0,0,2,0,216,1,3,5,188,5,67,
0,25,0,37,0,0,1,6,6,7,54,51,50,4,22,22,21,20,2,7,39,18,55,38,36,35,34,4,7,
54,18,55,0,6,35,34,38,53,52,54,51,50,22,21,2,0,33,50,12,156,223,132,1,45,
208,31,73,85,156,115,16,124,254,221,123,138,254,231,112,12,77,43,2,68,94,
65,65,94,93,66,66,93,5,3,73,161,56,18,35,47,27,22,135,254,210,184,79,1,3,
193,26,30,22,21,128,1,47,113,252,217,67,67,47,47,68,68,47,0,0,1,0,150,255,
206,5,113,7,52,0,32,0,0,19,54,51,50,23,18,19,23,2,2,21,20,22,51,50,55,23,
6,35,34,39,6,0,7,39,54,18,53,52,38,35,34,7,194,121,94,150,93,51,179,165,121,
123,101,128,98,207,56,233,148,184,95,73,254,227,143,82,229,249,108,131,54,
88,4,138,34,74,1,84,1,126,58,255,0,254,29,207,205,149,77,153,89,111,139,254,
244,75,154,138,1,43,136,199,155,24,0,0,2,0,216,255,175,5,188,6,11,0,44,0,
54,0,0,19,54,18,55,23,6,6,7,54,51,50,4,22,22,21,20,3,23,22,22,21,20,2,7,39,
18,55,38,36,35,34,7,39,54,33,50,22,23,54,55,38,36,35,34,4,23,39,54,51,50,
23,7,38,35,34,216,12,77,43,164,33,50,12,147,232,132,1,45,208,31,99,30,30,
18,73,85,156,115,16,124,254,221,123,238,204,20,237,1,18,94,222,103,78,13,
124,254,221,123,140,254,232,255,7,81,83,108,123,23,129,128,36,3,235,128,1,
47,113,64,73,161,56,17,35,47,27,22,222,254,253,8,9,25,16,135,254,210,184,
79,1,3,193,26,30,30,163,32,19,17,199,155,26,30,22,238,164,3,12,166,13,0,0,
0,0,0,0,15,0,186,0,3,0,1,4,9,0,0,0,94,0,0,0,3,0,1,4,9,0,1,0,52,0,94,0,3,0,
1,4,9,0,2,0,14,0,146,0,3,0,1,4,9,0,3,0,90,0,160,0,3,0,1,4,9,0,4,0,52,0,94,
0,3,0,1,4,9,0,5,0,30,0,250,0,3,0,1,4,9,0,6,0,46,1,24,0,3,0,1,4,9,0,7,0,68,
1,70,0,3,0,1,4,9,0,8,0,42,1,138,0,3,0,1,4,9,0,9,0,40,1,180,0,3,0,1,4,9,0,
10,0,96,1,220,0,3,0,1,4,9,0,11,0,62,2,60,0,3,0,1,4,9,0,12,0,60,2,122,0,3,
0,1,4,9,0,13,2,150,2,182,0,3,0,1,4,9,0,14,0,52,5,76,0,67,0,111,0,112,0,121,
0,114,0,105,0,103,0,104,0,116,0,32,0,50,0,48,0,49,0,49,0,32,0,71,0,111,0,
111,0,103,0,108,0,101,0,32,0,73,0,110,0,99,0,46,0,32,0,65,0,108,0,108,0,32,
0,82,0,105,0,103,0,104,0,116,0,115,0,32,0,82,0,101,0,115,0,101,0,114,0,118,
0,101,0,100,0,46,0,78,0,111,0,116,0,111,0,32,0,83,0,97,0,110,0,115,0,32,0,
73,0,109,0,112,0,101,0,114,0,105,0,97,0,108,0,32,0,65,0,114,0,97,0,109,0,
97,0,105,0,99,0,82,0,101,0,103,0,117,0,108,0,97,0,114,0,77,0,111,0,110,0,
111,0,116,0,121,0,112,0,101,0,32,0,73,0,109,0,97,0,103,0,105,0,110,0,103,
0,32,0,45,0,32,0,78,0,111,0,116,0,111,0,32,0,83,0,97,0,110,0,115,0,32,0,73,
0,109,0,112,0,101,0,114,0,105,0,97,0,108,0,32,0,65,0,114,0,97,0,109,0,97,
0,105,0,99,0,86,0,101,0,114,0,115,0,105,0,111,0,110,0,32,0,49,0,46,0,48,0,
51,0,32,0,117,0,104,0,78,0,111,0,116,0,111,0,83,0,97,0,110,0,115,0,73,0,109,
0,112,0,101,0,114,0,105,0,97,0,108,0,65,0,114,0,97,0,109,0,97,0,105,0,99,
0,78,0,111,0,116,0,111,0,32,0,105,0,115,0,32,0,97,0,32,0,116,0,114,0,97,0,
100,0,101,0,109,0,97,0,114,0,107,0,32,0,111,0,102,0,32,0,71,0,111,0,111,0,
103,0,108,0,101,0,32,0,73,0,110,0,99,0,46,0,77,0,111,0,110,0,111,0,116,0,
121,0,112,0,101,0,32,0,73,0,109,0,97,0,103,0,105,0,110,0,103,0,32,0,73,0,
110,0,99,0,46,0,77,0,111,0,110,0,111,0,116,0,121,0,112,0,101,0,32,0,68,0,
101,0,115,0,105,0,103,0,110,0,32,0,84,0,101,0,97,0,109,0,68,0,97,0,116,0,
97,0,32,0,117,0,110,0,104,0,105,0,110,0,116,0,101,0,100,0,46,0,32,0,68,0,
101,0,115,0,105,0,103,0,110,0,101,0,100,0,32,0,98,0,121,0,32,0,77,0,111,0,
110,0,111,0,116,0,121,0,112,0,101,0,32,0,100,0,101,0,115,0,105,0,103,0,110,
0,32,0,116,0,101,0,97,0,109,0,46,0,104,0,116,0,116,0,112,0,58,0,47,0,47,0,
119,0,119,0,119,0,46,0,103,0,111,0,111,0,103,0,108,0,101,0,46,0,99,0,111,
0,109,0,47,0,103,0,101,0,116,0,47,0,110,0,111,0,116,0,111,0,47,0,104,0,116,
0,116,0,112,0,58,0,47,0,47,0,119,0,119,0,119,0,46,0,109,0,111,0,110,0,111,
0,116,0,121,0,112,0,101,0,46,0,99,0,111,0,109,0,47,0,115,0,116,0,117,0,100,
0,105,0,111,0,84,0,104,0,105,0,115,0,32,0,70,0,111,0,110,0,116,0,32,0,83,
0,111,0,102,0,116,0,119,0,97,0,114,0,101,0,32,0,105,0,115,0,32,0,108,0,105,
0,99,0,101,0,110,0,115,0,101,0,100,0,32,0,117,0,110,0,100,0,101,0,114,0,32,
0,116,0,104,0,101,0,32,0,83,0,73,0,76,0,32,0,79,0,112,0,101,0,110,0,32,0,
70,0,111,0,110,0,116,0,32,0,76,0,105,0,99,0,101,0,110,0,115,0,101,0,44,0,
32,0,86,0,101,0,114,0,115,0,105,0,111,0,110,0,32,0,49,0,46,0,49,0,46,0,32,
0,84,0,104,0,105,0,115,0,32,0,70,0,111,0,110,0,116,0,32,0,83,0,111,0,102,
0,116,0,119,0,97,0,114,0,101,0,32,0,105,0,115,0,32,0,100,0,105,0,115,0,116,
0,114,0,105,0,98,0,117,0,116,0,101,0,100,0,32,0,111,0,110,0,32,0,97,0,110,
0,32,0,34,0,65,0,83,0,32,0,73,0,83,0,34,0,32,0,66,0,65,0,83,0,73,0,83,0,44,
0,32,0,87,0,73,0,84,0,72,0,79,0,85,0,84,0,32,0,87,0,65,0,82,0,82,0,65,0,78,
0,84,0,73,0,69,0,83,0,32,0,79,0,82,0,32,0,67,0,79,0,78,0,68,0,73,0,84,0,73,
0,79,0,78,0,83,0,32,0,79,0,70,0,32,0,65,0,78,0,89,0,32,0,75,0,73,0,78,0,68,
0,44,0,32,0,101,0,105,0,116,0,104,0,101,0,114,0,32,0,101,0,120,0,112,0,114,
0,101,0,115,0,115,0,32,0,111,0,114,0,32,0,105,0,109,0,112,0,108,0,105,0,101,
0,100,0,46,0,32,0,83,0,101,0,101,0,32,0,116,0,104,0,101,0,32,0,83,0,73,0,
76,0,32,0,79,0,112,0,101,0,110,0,32,0,70,0,111,0,110,0,116,0,32,0,76,0,105,
0,99,0,101,0,110,0,115,0,101,0,32,0,102,0,111,0,114,0,32,0,116,0,104,0,101,
0,32,0,115,0,112,0,101,0,99,0,105,0,102,0,105,0,99,0,32,0,108,0,97,0,110,
0,103,0,117,0,97,0,103,0,101,0,44,0,32,0,112,0,101,0,114,0,109,0,105,0,115,
0,115,0,105,0,111,0,110,0,115,0,32,0,97,0,110,0,100,0,32,0,108,0,105,0,109,
0,105,0,116,0,97,0,116,0,105,0,111,0,110,0,115,0,32,0,103,0,111,0,118,0,101,
0,114,0,110,0,105,0,110,0,103,0,32,0,121,0,111,0,117,0,114,0,32,0,117,0,115,
0,101,0,32,0,111,0,102,0,32,0,116,0,104,0,105,0,115,0,32,0,70,0,111,0,110,
0,116,0,32,0,83,0,111,0,102,0,116,0,119,0,97,0,114,0,101,0,46,0,104,0,116,
0,116,0,112,0,58,0,47,0,47,0,115,0,99,0,114,0,105,0,112,0,116,0,115,0,46,
0,115,0,105,0,108,0,46,0,111,0,114,0,103,0,47,0,79,0,70,0,76,0,0,0,3,0,0,
0,0,0,0,255,102,0,102,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,3,0,8,
0,2,0,14,0,3,255,255,0,3,};
#endif