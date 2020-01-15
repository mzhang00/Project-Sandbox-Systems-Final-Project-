#include "display.h"
#include <stdlib.h>


int init()
{
    //Initialization flag
    int success = 1;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = 0;
    }

    return success;
}

int loadMedia(SDL_Surface * gXOut){
    //Loading success flag
    int success = 1;
    char cwd[100];
    getcwd(cwd, 100);
    strcat(cwd, "/map.bmp");
    //Load splash image
    gXOut = SDL_LoadBMP(cwd );
    if( gXOut == NULL ){
        printf( "Unable to load image %s! SDL Error: %s\n", cwd, SDL_GetError() );
        success = 0;
    }
    return success;
}

void close1(SDL_Renderer * rend,SDL_Texture * tex, SDL_Window * win ){
    SDL_DestroyTexture(tex);
    tex = NULL;

    SDL_DestroyRenderer(rend);
    rend = NULL;
    //Destroy window
    SDL_DestroyWindow( win );
    win = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

void clear(SDL_Renderer * renderer) {
    //SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void render(SDL_Renderer* rend, SDL_Texture * tex, SDL_Rect *rect, SDL_Texture * tex2, SDL_Rect *rect2, SDL_Texture * tex3, SDL_Rect * rect3, SDL_Rect * rect4, struct unit * units) {
  SDL_RenderClear(rend);
  SDL_RenderCopy(rend, tex2, NULL, rect2);
  int i;
  for (i = 0; i < 6;i++) {
    SDL_RenderCopy(rend, tex, NULL, &(rect[i]));
    //Set Render color to black
    SDL_SetRenderDrawColor( rend, 0, 0, 0, 255 );
    SDL_RenderFillRect( rend, &(rect4[i]));
    SDL_Rect copyRect = rect4[i];
    copyRect.w = units[i].health;
    // Set render color to red
    SDL_SetRenderDrawColor( rend, 255, 0, 0, 255 );
    SDL_RenderFillRect( rend, &(copyRect));
  }
  SDL_RenderCopy(rend, tex3, NULL, rect3);

  // triggers the double buffers
  // for multiple rendering
  SDL_RenderPresent(rend);

  // calculates to 60 fps
  SDL_Delay(1000 / 60);
}
