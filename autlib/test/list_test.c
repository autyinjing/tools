/* ***********************************************************************

  > File Name: list_test.c
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年04月26日 星期三 11时32分54秒

 ********************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../list.h"

struct Player {
    int id;
    struct list_head game_list;
};

struct Game {
    int     id;
    char    name[8];
    struct list_head next;
};

void setGameInfo(struct Player *pPlayer)
{
    char game_name[5][8] = {"cr", "lol", "wow", "dnf", "snake"};
    struct Game *new_game = NULL;
    for (int i = 0; i < 5; ++i)
    {
        new_game = (struct Game *)malloc(sizeof(struct Game));
        new_game->id = i + 1;
        strncpy(new_game->name, game_name[i], strlen(game_name[i]) + 1);
        INIT_LIST_HEAD(&new_game->next);
        list_add(&new_game->next, &pPlayer->game_list);
        /*list_add_tail(&new_game->next, &pPlayer->game_list);*/
        if (list_is_singular(&pPlayer->game_list))
            printf("list_is_singular ! i = %d\n", i);
        new_game = NULL;
    }
}

void printGame(struct Game *pGame)
{
    printf("id:%d   name:%s\n", pGame->id, pGame->name);
}

void printGameInfo(struct Player *pPlayer)
{
    struct Game *pGame = NULL;
    list_for_each_entry(pGame, &pPlayer->game_list, next)
    /*list_for_each_entry_reverse(pGame, &pPlayer->game_list, next)*/
    {
        printGame(pGame);
    }
}

void cutTest(struct Player *pPlayer)
{
    LIST_HEAD(list);
    struct list_head *pos = NULL;
    struct Game *pGame = NULL;
    list_for_each(pos, &pPlayer->game_list)
    {
        pGame = list_entry(pos, struct Game, next);
        if (!strcmp(pGame->name, "dnf")) {
            printf("Found dnf!\n");
            break;
        }
    }

    list_cut_position(&list, &pPlayer->game_list, pos);
    /*list_cut_position(&list, &pPlayer->game_list, &pPlayer->game_list);*/
    /*LIST_HEAD(empty_list);*/
    /*list_cut_position(&list, &empty_list, pos);*/
    printf("after list_cut_position:\n");
    printGameInfo(pPlayer);

    /*printf("after list_splice:\n");*/
    /*list_splice(&list, &pPlayer->game_list);*/
    printf("after list_splice_tail:\n");
    list_splice_tail(&list, &pPlayer->game_list);
    printGameInfo(pPlayer);
}

void delTest(struct Player *pPlayer)
{
    struct list_head *pos = NULL;
    struct Game *pGame = NULL;
    list_for_each(pos, &pPlayer->game_list)
    {
        pGame = list_entry(pos, struct Game, next);
        if (!strcmp(pGame->name, "dnf")) {
            printf("Found dnf!\n");
            break;
        }
    }
    /*list_del(pos);*/
    /*printf("after list_del:\n");*/
    /*printGameInfo(pPlayer);*/

    /*struct Game new_game;*/
    /*new_game.id = 11;*/
    /*strncpy(new_game.name, "LCK", 4);*/
    /*INIT_LIST_HEAD(&new_game.next);*/
    /*list_replace(pos, &new_game.next);*/
    /*printf("after list_replace:\n");*/
    /*printGameInfo(pPlayer);*/

    if (list_is_last(pos, &pPlayer->game_list))
        printf("cr is last !\n");
    else
        printf("dnf is not last !\n");
}

int main(int argc, const char* argv[])
{
    struct Player player;

    INIT_LIST_HEAD(&player.game_list);

    if (list_empty(&player.game_list))
        printf("game_list is empty()!\n");

    setGameInfo(&player);

    printf("begin:\n");
    printGameInfo(&player);

    /*list_rotate_left(&player.game_list);*/
    /*printf("after rotate_left:\n");*/
    /*printGameInfo(&player);*/

    /*cutTest(&player);*/

    delTest(&player);

    return 0;
}
