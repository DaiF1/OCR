#include "saveLoad.h"

void save(Matrix *hw, Matrix *hb, Matrix *ow, Matrix *ob, char* filename)
{
FILE *fp;
fp = fopen(filename, "w");



int row = hw -> rows;
int col = hw -> cols;

fprintf(fp, "%d\n", row);
fprintf(fp, "%d\n", col);

    
    for (int i = 0; i < hw->rows; i++)
    {
        for (int j = 0; j < hw->cols; j++)
        {
            fprintf(fp,"%f\n", m_get(hw, i, j));
        }
    }
    
row = hb -> rows;
col = hb -> cols;

fprintf(fp, "%d\n", row);
fprintf(fp, "%d\n", col);

    
    for (int i = 0; i < hb->rows; i++)
    {
        for (int j = 0; j < hb->cols; j++)
        {
            fprintf(fp,"%f\n", m_get(hb, i, j));
        }
    }

row = ow -> rows;
col = ow -> cols;

fprintf(fp, "%d\n", row);
fprintf(fp, "%d\n", col);

    
    for (int i = 0; i < ow->rows; i++)
    {
        for (int j = 0; j < ow->cols; j++)
        {
            fprintf(fp,"%f\n", m_get(ow, i, j));
        }
    }

row = ob -> rows;
col = ob -> cols;

fprintf(fp, "%d\n", row);
fprintf(fp, "%d\n", col);

    
    for (int i = 0; i < ob->rows; i++)
    {
        for (int j = 0; j < ob->cols; j++)
        {
            fprintf(fp,"%f\n", m_get(ob, i, j));
        }
    }
    


fclose(fp);
}

int load(char *filename, Matrix *hw, Matrix *hb, Matrix *ow, Matrix *ob)
{
    int retur = 0;
    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL)
        return 1;

    int num_inputs;
    int num_hidden;
    int num_outputs;

    retur = fscanf(fp, "%d", &num_inputs);
    retur = fscanf(fp, "%d", &num_hidden);

    m_init(hw, num_inputs, num_hidden);

    double tmp = 0.00;

    for (int i = 0; i < hw->rows; i++)
    {
        for (int j = 0; j < hw->cols; j++)
        {
            retur = fscanf(fp, "%lf", &tmp);
            m_setIndex(hw, i, j, tmp);
        }
    }

    retur = fscanf(fp, "%d", &num_inputs);
    retur = fscanf(fp, "%d", &num_hidden);

    m_init(hb, 1, num_hidden);

    for (int i = 0; i < hb->rows; i++)
    {
        for (int j = 0; j < hb->cols; j++)
        {
            retur = fscanf(fp, "%lf", &tmp);
            m_setIndex(hb, i, j, tmp);
        }
    }

    retur = fscanf(fp, "%d", &num_hidden);
    retur = fscanf(fp, "%d", &num_outputs);

    m_init(ow, num_hidden, num_outputs);

    for (int i = 0; i < ow->rows; i++)
    {
        for (int j = 0; j < ow->cols; j++)
        {
            retur = fscanf(fp, "%lf", &tmp);
            m_setIndex(ow, i, j, tmp);
        }
    }

    retur = fscanf(fp, "%d", &num_hidden);
    retur = fscanf(fp, "%d", &num_outputs);

    m_init(ob, num_hidden, num_outputs);

    for (int i = 0; i < ob->rows; i++)
    {
        for (int j = 0; j < ob->cols; j++)
        {
            retur = fscanf(fp, "%lf", &tmp);
            m_setIndex(ob, i, j, tmp);
        }
    }

    fclose(fp);

    return retur;
}
