
#include <stdio.h>

#define SIZE 5

// direction vectors
const int dx[] = {+1, 0, -1, 0};
const int dy[] = {0, +1, 0, -1};

// matrix dimensions
//int row_count = SIZE;
//int col_count= SIZE;

// the input matrix
int m[SIZE][SIZE] =
{
    { 0, 0, 0, 1, 1},
    { 0, 1, 0, 1, 0},
    { 0, 0, 0, 0, 0},
    { 0, 0, 0, 1, 1},
    { 0, 1, 0, 1, 0}
};

// the labels, 0 means unlabeled
int label[SIZE][SIZE];

int dfs(int x, int y, int current_label)
{
    if (x < 0 || x == SIZE)
        return 0; // out of bounds

    if (y < 0 || y == SIZE)
        return 0; // out of bounds
    
    if (label[x][y] || !m[x][y])
    {
        return 0; // already labeled or not marked with 1 in m
    }
    
    // mark the current cell
    label[x][y] = current_label;
    
    // recursively mark the neighbors
    int acc = 0;
    for (int direction = 0; direction < 4; ++direction)
    {
        acc += dfs(x + dx[direction], y + dy[direction], current_label);
    }

    printf("\nacc %i , label %i" , acc ,current_label );
    
    return 1;
}

void find_components()
{
    int component = 0;
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if (!label[i][j] && m[i][j])
                dfs(i, j, ++component);

        }
    }
}


int main()
{
    for (int i =0 ; i<SIZE; i++)
        for (int j =0 ; j<SIZE; j++)
        {
            label[i][j] = 0;
        }
    
    find_components();
    
    printf("\n");
    
    for (int i = 0; i < SIZE ; i++)
    {
        
        for (int j = 0; j < SIZE ; j++)
            printf(" %i " , label[i][j]);
        
        printf("\n");
    }
    
    
    return 0;
}