/*
 * Buzzer_Driver.c
 *
 * Class to hold buzzer methods, used to activate a 668-1468-ND Buzzer.
 *
 *  Author: seanw
 */

 /* Includes */
 #include <asf.h>
 #include "Drivers/Outputs/Buzzer/Buzzer_Driver.h"

 /* Global Variable Declarations */

 /*******************************************************************************************/
							/* Simple Buzzer methods */

 /* Buzzer method to start buzzer @ a certain period. */
 void buzzer(uint16_t period){
	TCD0.CTRLA = 0b00000110; //prescalar 256
	 PORTD.OUT |= 0b00001000; //out, high = 1, low = 0.

	 TCD0.PER = period; //Set period of output.
	 TCD0.CCD = TCD0.PER;	//Set duty cycle of output for Command Capture register A.
 }


 /*******************************************************************************************/
 						/* Music methods for testing*/ 
 /* Length of Song in no. of notes & rests*/
 //int introSongLength;
 //int songLength;
 
 /*Notes*/
 //char introNotes[];
 //char notes[]; // Notes is an array of text characters corresponding to the notes in your song. A space represents a rest (no tone)
 
 /*Beats*/
 //int introBeats[];
 //int beats[]; //an array of values for each note and rest. A "1" represents a quarter-note, 2 a half-note, etc. Don't forget that the rests (spaces) need a length as well.

 /*tempo*/
 //int introTempo;
 //int tempo; //how fast to play the song. To make the song play faster, decrease this value.

 /* Camptown Races *//*
 int introSongLength = 18;
 int songLength = 46;
 
 char introNotes[] = "cdfdA AG cdfdG Gf ";
 char notes[] = "ddbdedbba ba ddbdedbabagggbdGeeGedddbdedbacbag";

 int introBeats[] = {1,1,1,1,1,1,4,4,2,1,1,1,1,1,1,4,4,2};
 int beats[] = {1,1,1,1,1,1,2,1,3,1,1,3,1,1,1,1,1,1,1,2,2,1,1,4,1,1,1,1,4,1,1,1,1,4,1,1,1,1,1,1,2,1,1,1,1,4};

 int introTempo = 150;
 int tempo = 200;
 */
 /*Mario*//*
 int introSongLength = ;
 int songLength = ;
 
 char introNotes[] = "";
 char notes[] = ""; 

 int introBeats[] = {};
 int beats[] = {};

 int introTempo = ;
 int tempo = ;
 *//*
 void music() {

	 playMusic(introSongLength,introBeats,introTempo,introNotes); //intro
	 delay_ms(500);

	 playMusic(songLength,beats,tempo,notes); //camptown races
	 delay_ms(500);

	 buzzer(196);	//tone(buzzerPin, 196, 3000);
	 delay_ms(3000);

	 PORTD.OUT = 0b00000000;
 }

 void playMusic(int songLength, int* beats, int tempo, char* notes) {
	 int i, duration;

	 for (i = 0; i < songLength; i++) {// step through the song arrays
		 duration = beats[i] * tempo;  // length of note/rest in ms

		 if (notes[i] == ' ') {          // is this a rest?
			 delay_ms(duration);            // then pause for a moment
		 } 
		 else {                         // otherwise, play the note
			 buzzer(frequency(notes[i]));
			 LED(1249,100);
			 delay_ms(duration);            // wait for tone to finish
			 PORTD.OUT = 0b00000000;    //turn both off
		 }
		 delay_ms(tempo/10);              // brief pause between notes
	 }
 }

 int frequency(char note) {
	 // This function takes a note character (a-g), and returns corresponding frequency in Hz for the tone() function.

	 int i;
	 const int numNotes = 11;  // number of notes we're storing

	  * The following arrays hold the note characters and corresponding frequencies. 
	  * The last "C" note is  to separate it from the first lowercase "c". 
	  * If you want add more notes, you'll need to use unique characters.
	  *  
		For the "char" (character) type, we put single in single quotes.
	  *//*

	 char names[] = { 'g', 'a', 'b', 'c', 'd', 'e', 'f', 'G', 'A', 'B', 'C' };
	 int frequencies[] = {196, 220, 247, 262, 294, 330, 349, 392, 440, 494, 523};

	 // Now we'll search through the letters in the array, and if we find it, we'll return the frequency for that note.

	 for (i = 0; i < numNotes; i++){  // Step through the notes
		 if (names[i] == note) {         // Is this the one?
			 return(frequencies[i]);     // Yes! Return the frequency
		 }
	 }
	 return 0;
}
*/