#define NOTES_IN_SCALE 8 
#define TRIAD 3 

/* octave 0 */
const int A0  =   27;
const int Bf0 =   29;
const int As0 =   29;

/* octave 1 */
const int B0  =   31;
const int C1  =   33;
const int Df1 =   35;
const int Cs1 =   35;
const int D1  =   37;
const int Ef1 =   39;
const int Ds1 =   39;
const int E1  =   41;
const int F1  =   44;
const int Gf1 =   46;
const int Fs1 =   46;
const int G1  =   49;
const int Af1 =   52;
const int Gs1 =   52;
const int A1  =   55;
const int Bf1 =   58;
const int As1 =   58;
const int B1  =   62;

/* octave 2 */
const int C2  =   65;
const int Df2 =   69;
const int Cs2 =   69;
const int D2  =   73;
const int Ef2 =   78;
const int Ds2 =   78;
const int E2  =   82;
const int F2  =   87;
const int Gf2 =   92;
const int Fs2 =   92;
const int G2  =   98;
const int Af2 =  104;
const int Gs2 =  104;
const int A2  =  110;
const int Bf2 =  117;
const int As2 =  117;
const int B2  =  123;

/* octave 3 */
const int C3  =  130;
const int Df3 =  138;
const int Cs3 =  138;
const int D3  =  147;
const int Ef3 =  155;
const int Ds3 =  155;
const int E3  =  165;
const int F3  =  175;
const int Gf3 =  185;
const int Fs3 =  185;
const int G3  =  196;
const int Af3 =  208;
const int Gs3 =  208;
const int A3  =  220;
const int Bf3 =  233;
const int As3 =  233;
const int B3  =  247;

/* octave 4 */
const int C4  =  262;
const int Df4 =  277;
const int Cs4 =  277;
const int D4  =  294;
const int Ef4 =  311;
const int Ds4 =  311;
const int E4  =  330;
const int F4  =  349;
const int Gf4 =  370;
const int Fs4 =  370;
const int G4  =  392;
const int Gs4 =  415;
const int Af4 =  415;
const int A4  =  440;
const int Bf4 =  466;
const int As4 =  466; 
const int B4  =  493;

/* octave 5 */
const int C5  =  523;
const int Df5 =  554;
const int Cs5 =  554; 
const int D5  =  587;
const int Ef5 =  622;
const int Ds5 =  622;
const int E5  =  659;
const int F5  =  698;
const int Gf5 =  739;
const int Fs5 =  739; 
const int G5  =  783;
const int Af5 =  830;
const int Gs5 =  830;
const int A5  =  880;
const int Bf5 =  932;
const int As5 =  932;
const int B5  =  988;

/* octave 6 */
const int C6  = 1047;
const int Df6 = 1109;
const int Cs6 = 1109;
const int D6  = 1175;
const int Ef6 = 1245;
const int Ds6 = 1245;
const int E6  = 1319;
const int F6  = 1397;
const int Gf6 = 1480;
const int Fs6 = 1480;
const int G6  = 1568;
const int Af6 = 1661;
const int Gs6 = 1661;
const int A6  = 1760;
const int Bf6 = 1865;
const int As6 = 1865;
const int B6  = 1976;

/* octave 7 */
const int C7  = 2093;
const int Df7 = 2217;
const int Cs7 = 2217;
const int D7  = 2349;
const int Ef7 = 2489;
const int Ds7 = 2489;
const int E7  = 2637;
const int F7  = 2794;
const int Gf7 = 2960;
const int Fs7 = 2960;
const int G7  = 3136;
const int Af7 = 3322;
const int Gs7 = 3322;
const int A7  = 3520;
const int Bf7 = 3729;
const int As7 = 3729;
const int B7  = 3951;

/* octave 8 */
const int C8  = 4186;

/* Durations, in milliseconds */
const int sixteenth = quarter / 4; 
const int eighth = quarter / 2; 
const int quarter = 1000; 
const int half = quarter * 2; 
const int whole = quarter * 4; 

/* octave 4 */
const int C4  =  262;
const int Df4 =  277;
const int Cs4 =  277;
const int D4  =  294;
const int Ef4 =  311;
const int Ds4 =  311;
const int E4  =  330;
const int F4  =  349;
const int Gf4 =  370;
const int Fs4 =  370;
const int G4  =  392;
const int Gs4 =  415;
const int Af4 =  415;
const int A4  =  440;
const int Bf4 =  466;
const int As4 =  466; 
const int B4  =  493;

/* Scales 
   Uses octaves 4-6 
   8 pitches per scale 
   Includes every semitone major/minor scale 
*/
int C_Major[NOTES_IN_SCALE] = {C4, D4, E4, F4, G4, A4, B4, C5, D5, E5, F5, G5}; 
int C_Minor[NOTES_IN_SCALE] = {C4, D4, Ef4, F4, G4, Af4, Bf4, C5, D5, Ef5, F5, G5}; 
int C_Sharp_Major[NOTES_IN_SCALE] = {Cs4, Ds4, F4, Fs4, Gs4, As4, C5, Cs5, Ds5, F5, Fs5, Gs5}; 
int C_Sharp_Minor[NOTES_IN_SCALE] = {Cs4, Ds4, E4, Fs4, Gs4, A4, B5, Cs5, Ds5, E5, Fs5, Gs5}; 
int D_Major[NOTES_IN_SCALE] = {D4, E4, Fs4, G4, A4, B4, Cs5, D5, E5, Fs5, G5, A5}; 
int D_Minor[NOTES_IN_SCALE] = {D4, E4, F4, G4, A4, Bf4, C5, D5, E5, F5, G5, A5}; 
int E_Flat_Major[NOTES_IN_SCALE] = {Ef4, F4, G4, Af4, Bf4, C5, D5, Ef5, F5, G5, Af5, Bf5}; 
int E_Flat_Minor[NOTES_IN_SCALE] = {Ef4, F4, Gf4, Af4, Bf4, B4, Df5, Ef5, F5, Gf5, Af5, Bf5}; 
int E_Major[NOTES_IN_SCALE] = {E4, Fs4, Gs4, A4, B4, Cs5, Ds5, E5, Fs5, Gs5, A5, B5}; 
int E_Minor[NOTES_IN_SCALE] = {E4, Fs4, G4, A4, B4, C5, D5, E5, Fs5, G5, A5, B5}; 
int F_Major[NOTES_IN_SCALE] = {F4, G4, A4, Bf4, C5, D5, E5, F5, G5, A5, Bf5, C6}; 
int F_Minor[NOTES_IN_SCALE] = {F4, G4, Af4, Bf4, C5, Df5, Ef5, F5, G5, Af5, Bf5, C6}; 
int G_Flat_Major[NOTES_IN_SCALE] = {Gf4, Af4, Bf4, B5, Df5, Ef5, F5, Gf5}; 
int G_Flat_Minor[NOTES_IN_SCALE] = {Gf4, Af4, A4, B5, Df5, D5, E5, Gf5}; 
int G_Major[NOTES_IN_SCALE] = {G4, A4, B4, C5, D5, E5, Fs5, G5}; 
int G_Minor[NOTES_IN_SCALE] = {G4, A4, Bf4, C5, D5, Ef5, F5, G5}; 
int A_Flat_Major[NOTES_IN_SCALE] = {Af4, Bf4, C5, Df5, Ef5, F5, G5, Af5}; 
int A_Flat_Minor[NOTES_IN_SCALE] = {Af4, Bf4, B5, Df5, Ef5, E5, Gf5, Af5}; 
int A_Major[NOTES_IN_SCALE] = {A4, B4, Cs5, D5, E5, Fs5, Gs5, A5}; 
int A_Minor[NOTES_IN_SCALE] = {A4, B4, C5, D5, E5, F5, G5, A5}; 
int B_Flat_Major[NOTES_IN_SCALE] = {Bf4, C5, D5, Ef5, F5, G5, A5, Bf5};   
int B_Flat_Minor[NOTES_IN_SCALE] = {Bf4, C5, Df5, Ef5, F5, Gf5, Af5, Bf5};   
int B_Major[NOTES_IN_SCALE] = {B4, Cs5, Ds5, E5, Fs5, Gs5, As5, B5}; 
int B_Minor[NOTES_IN_SCALE] = {B4, Cs5, D5, E5, Fs5, G5, A5, B5}; 

/* Chords */
int i[TRIAD] = {0, 2, 4}; 
int ii[TRIAD] = {1, 3, 5}; 
int iii[TRIAD] = {2, 4, 6}; 
int iv[TRIAD] = {3, 5, 7}; 
int v[TRIAD] = {4, 6, 8}; 
int vi[TRIAD] = {5, 7, 9}; 
int vii[TRIAD] = {6, 8, 10}; 
int viii[TRIAD] = {7, 9, 11}; 