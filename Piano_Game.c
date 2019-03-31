#include <stdbool.h>
#include <stdlib.h>


volatile int pixel_buffer_start; // global variable 
void wait_for_vsync();
void plot_pixel(int x, int y, short int line_color);
void clear_screen();

void draw_grid(); 
void draw_vertical_line(int x);

void draw_square(int x , int y);
//helper functions to determine music box
void note_to_draw(int music);
void clear_music();

////////////////////////////////////Gobal Variables/////////////////////////////////
int Music[5] = {0b1000000,
				0b0100000,
				0b0010000,
				0b1010000,
				0b1111111};

int Music_index = 0;
bool next_note = true;

//these are position of squares
    int do_x = 0;
    int do_y = 500; //make it not plot on screen
    
    int re_x = 45;
    int re_y = 500;

    int mi_x = 90;
    int mi_y = 500;

    int fa_x = 135;
    int fa_y = 500;

    int so_x = 180;
    int so_y = 500;

    int la_x = 225;
    int la_y = 500;

    int si_x = 270;
    int si_y = 500;

    int speed = 15;//speed of block move 

////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;

    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer

    while (1)
    {
        /* Erase any boxes and lines that were drawn in the last iteration */
        clear_screen();
        // code for drawing the boxes and lines (not shown)
        draw_grid();

        if(next_note == true){
        	next_note = false;
        	if(Music_index == 5){    ////to see music is end
        		Music_index = 0;
        	}

        	int music = Music[Music_index];
        	note_to_draw(music);
        }

        draw_square(do_x , do_y);
        draw_square(re_x , re_y);
        draw_square(mi_x , mi_y);
        draw_square(fa_x , fa_y);
        draw_square(so_x , so_y);
        draw_square(la_x , la_y);
        draw_square(si_x , si_y);
        
        
        // code for updating the locations of boxes (not shown)
        do_y = do_y + speed;
        re_y = re_y + speed;
        mi_y = mi_y + speed;
        fa_y = fa_y + speed;
        so_y = so_y + speed;
        la_y = la_y + speed;
        si_y = si_y + speed;

        //check for box reach the bottom

        if(((do_y > 240) && (do_y < 400))||
           ((re_y > 240) && (re_y < 400))||
           ((mi_y > 240) && (mi_y < 400))||
           ((fa_y > 240) && (fa_y < 400))||
           ((so_y > 240) && (so_y < 400))||
           ((la_y > 240) && (la_y < 400))||
           ((si_y > 240) && (si_y < 400))){
           	next_note = true;
            Music_index ++;
            clear_music();

        }	
        

        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
}



///////////////////////////////////////////////////////////////////////////////
void wait_for_vsync(){
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    int status;
    *pixel_ctrl_ptr = 1;
    status = *(pixel_ctrl_ptr + 3);
    while((status & 0x01)!= 0){
        status = *(pixel_ctrl_ptr + 3);
    }
}



void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
    return;
}



void clear_screen(){
    int x;
    int y;
    for (x=0; x<320; x++){  
        for(y =0; y<240; y++){
            plot_pixel(x, y, 0xFFFF); //white screen
        }
    }
    return ;
}


//////////////////////Draw grid////////////////////////////
void draw_grid(){
	draw_vertical_line(45);
	draw_vertical_line(90);
	draw_vertical_line(135);

	draw_vertical_line(180);
	draw_vertical_line(225);
	draw_vertical_line(270);

}

void draw_vertical_line(int x){
	int y;
	for (y=0; y<240; y++){
			plot_pixel(x, y, 0x0); //white screen
	}
}

///////////////////Draw Square////////////////////////////
void draw_square(int x , int y){
	int i;
    int j;
    for (i=0; i<45; i++){
        for(j =0; j<50; j++){
        	if((y+j) < 240 ){ plot_pixel(x+i, y+j,0x0); } //white screen
        }
    }
    return ;
}


///////////////////note to draw//////////////////////
void note_to_draw(int music){
	if((music & 0b1000000) == 0b1000000){
		do_y = 0;
	}
	if((music & 0b0100000) == 0b0100000){
		re_y = 0;
	}
	if((music & 0b0010000) == 0b0010000){
		mi_y = 0;
	}
	if((music & 0b0001000) == 0b0001000){
		fa_y = 0;
	}
	if((music & 0b0000100) == 0b0000100){
		so_y = 0;
	}
	if((music & 0b0000010) == 0b0000010){
		la_y = 0;
	}
	if((music & 0b0000001) == 0b0000001){
		si_y = 0;
	}
	return;
}

void clear_music(){
	//these are position of squares
    do_y = 500; //make it not plot on screen
    re_y = 500;
    mi_y = 500;
    fa_y = 500;
    so_y = 500;
    la_y = 500;
    si_y = 500;
    return;
}