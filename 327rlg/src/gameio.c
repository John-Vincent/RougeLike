#include "../headers/gameio.h"

/**
 * returns a string with the path to the file
 * @param  file_path               char pointer to be filled with the path
 * @return                         0 if successful -1 if not
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-02-01T22:52:22+000
 */
int getPath(char *file_path);

int saveMap(int numRooms, room_t const *rooms, uint8_t map_hard[mapHeight][mapWidth]){
  char *file_marker = "RLG327-S2017";
  uint32_t version = 0;
  uint32_t size = 16820 + (sizeof (*rooms) * numRooms);

  char file_path[100];

  getPath(file_path);

  FILE *dungeon = fopen(file_path, "w");
  if(dungeon == NULL){
    printf("failed to open file: %s\nerror: %s\n", file_path, strerror(errno));
    return -1;
  }

  version = htobe32(version);
  size = htobe32(size);

  if(fwrite(file_marker, 1, 12, dungeon) != 12){
    printf("failed to write file marker\nerror: %s\n", strerror(errno));
    return -1;
  }
  if(fwrite(&version, sizeof (version), 1, dungeon) != 1){
    printf("failed to write version\nerror: %s\n", strerror(errno));
    return -1;
  }
  if(fwrite(&size, sizeof (size), 1, dungeon) != 1){
    printf("failed to write size of file\nerror: %s\n", strerror(errno));
    return -1;
  }
  if(fwrite(map_hard, sizeof (**map_hard), mapHeight*mapWidth, dungeon) != mapHeight*mapWidth){
    printf("failed to write map hardness values\nerror: %s\n", strerror(errno));
    return -1;
  }
  if(fwrite(rooms, 4, numRooms, dungeon) != numRooms){
    printf("failed to write room values\nerror: %s\n", strerror(errno));
    return -1;
  }

  return fclose(dungeon);
}


int loadMap(room_t **rooms, int *numRooms, uint8_t map_hard[mapHeight][mapWidth]){
  char file_marker[12];
  uint32_t version;
  uint32_t size;
  errno = 0;

  char file_path[100];
  getPath(file_path);

  FILE *dungeon = fopen(file_path, "r");
  if(dungeon == NULL){
    printf("failed to open file: %s\nerror: %s\n", file_path, strerror(errno));
    return -1;
  }

  if(fread(file_marker, 1, 12, dungeon) != 12){
    printf("failed to read file marker\nerror: %s\n", strerror(errno));
    return -1;
  }
  if(fread(&version, sizeof (version), 1, dungeon) != 1){
    printf("failed to read version\nerror: %s\n", strerror(errno));
    return -1;
  }
  version = be32toh(version);
  if(fread(&size, sizeof (size), 1, dungeon) != 1){
    printf("failed to read size of file\nerror: %s\n", strerror(errno));
    return -1;
  }
  size = be32toh(size);
  if(fread(map_hard, sizeof (**map_hard), mapHeight*mapWidth, dungeon) != mapHeight*mapWidth){
    printf("failed to read map hardness values\nerror: %s\n", strerror(errno));
    return -1;
  }

  *numRooms = (size - 16820)/(sizeof (**rooms));
  *rooms = malloc(sizeof (**rooms) * (*numRooms));
  if(*rooms == NULL){
    printf("failed to allocate space for room data\nerror: %s\n", strerror(errno));
    return -1;
  }

  if(fread(*rooms, 4, *numRooms, dungeon) != *numRooms){
    printf("failed to read room values\nerror: %s\n", strerror(errno));
    return -1;
  }

  return fclose(dungeon);

}


int getPath(char *file_path){
  char *env = getenv("HOME");
  if(env == NULL){
    printf("could not get home directory\n");
    return -1;
  }
  strcpy(file_path, env);
  strcat(file_path, "/.rlg327/dungeon");
  return 0;
}
