#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <raylib.h>

#define ARRAY_LEN(xs) sizeof(xs) / sizeof(xs[0])

uint32_t global_frames[1024];
size_t global_frames_count = 0;

void callback(void *bufferData, unsigned int frames) {
  if (frames > ARRAY_LEN(global_frames)) {
    frames = ARRAY_LEN(global_frames);
  }

  memcpy(global_frames, bufferData, sizeof(uint32_t) * frames);
  global_frames_count = frames;
}

int main(void) {

  InitWindow(800, 800, "Music Visualizer");
  SetTargetFPS(60);

  InitAudioDevice();
  Music music = LoadMusicStream("Street Fighter II - V - Hadouken.mp3");
  PlayMusicStream(music);
  AttachAudioStreamProcessor(music.stream, callback);

  while (!WindowShouldClose()) {
    UpdateMusicStream(music);

    if (IsKeyPressed(KEY_SPACE)) {
      if (IsMusicStreamPlaying(music)) {
        PauseMusicStream(music);
      } else {
        ResumeMusicStream(music);
      }
    }

    float width = (float)GetRenderWidth();
    float height = (float)GetRenderHeight();

    BeginDrawing();
    ClearBackground(CLITERAL(Color){0x18, 0x18, 0x18, 0xFF});

    float cell_width = (float)width / global_frames_count;
    for (size_t i = 0; i < global_frames_count; i++) {
      int16_t sample = *(int16_t *)&global_frames[i];

      if (sample > 0) {
        float t = (float)sample / INT16_MAX;
        DrawRectangle(i * cell_width, height / 2 - height / 2 * t, cell_width,
                      height / 2 * t, RED);
      } else {
        float t = (float)sample / INT16_MIN;
        DrawRectangle(i * cell_width, height / 2, cell_width, height / 2 * t,
                      RED);
      }
    }

    EndDrawing();
  }

  return 0;
}
