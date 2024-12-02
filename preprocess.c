#include "types.h"
#include "stat.h"
#include "user.h"
char buf[512];
void organizeArgs(int argc, char *argv[], char **vars, char **vals)
{
    int i, j, k, inVar;
    for (i = 2; i < argc; i++)
    {
        // Verify that arg is valid
        if (argv[i][0] != '-' || argv[i][1] != 'D' ||
            ((65 > argv[i][2] || argv[i][2] > 90) && (97 > argv[i][2] || argv[i][2] > 122) && (argv[i][2] != '_')))
        {
            printf(1, "Invalid format for argument.\nArguments must have form -D<var>=<val>.\n");
            exit();
        }
        // Allocating memory for deep copies of arguments
        vars[i - 2] = malloc(strlen(argv[i]) * sizeof(char));
        vals[i - 2] = malloc(strlen(argv[i]) * sizeof(char));
        vars[i - 2][0] = argv[i][2];
        // 3 is first position of variable in -D<var>=<val>
        j = 3;
        k = 1;
        inVar = 1;
        while (argv[i][j] != '\0')
        {
            char c = argv[i][j++];
            if (inVar)
            {
                if ((48 <= c && c <= 57) || (65 <= c && c <= 90) || (97 <= c && c <= 122) || (c == '_'))
                    vars[i - 2][k++] = c;
                else if (c == '=')
                {
                    inVar = 0;
                    k = 0;
                }
                else
                {
                    printf(1, "Invalid format for argument.\nArguments must have form -D<var>=<val>.\n");
                    exit();
                }
            }
            else
                vals[i - 2][k++] = c;
        }
    }
    for (i = 2; i < argc; i++)
    {
        int j;
        for (j = i + 1; j < argc; j++)
        {
            if (strcmp(vars[i - 2], vars[j - 2]) == 0)
            {
                printf(1, "preprocess : repeated definition\n");
                exit();
            }
        }
    }
}
int checkVar(int *pos, int *activeVarCount, char c, char *var, char *val, int *inVar, int varCount)
{
    int k;
    if (*pos + 1 == strlen(var))
    {
        // Check if c is not a character in a valid C identifier
        if ((48 > c || c > 57) && (65 > c || c > 90) && (97 > c || c > 122) && (c != '_'))
        {
            (*activeVarCount)--;
            printf(1, "%s", val);
            // Clean up any other active vars
            for (k = 0; k < varCount; k++)
                inVar[k] = 0;
            *activeVarCount = 0;
        }
        else
        {
            (*activeVarCount)--;
            if (*activeVarCount == 0)
                printf(1, "%s", var);
        }
        *pos = 0;
    }
    else if (var[*pos + 1] == c)
    {
        // Positions (2 - varPositions[j]) have been verified in argv[j + 2]
        (*pos)++;
        return 1;
    }
    else
    {
        (*activeVarCount)--;
        if (*activeVarCount == 0)
            for (k = 0; k <= *pos; k++)
                printf(1, "%c", var[k]);
        *pos = 0;
    }
    return 0;
}
void commandPreprocess(int fd, int argc, char **vars, char **vals)
{
    int i, j, n;
    // bool value that tracks if function is passing through a non-arg string
    int inStr = 0;
    int activeVarCount = 0;
    // Pointer to bool values that track if function is passing through 1 or more args.
    int *inVars = malloc((argc - 2) * sizeof(int));
    // Pointer to int values that track current position in each arg.
    int *varPositions = malloc((argc - 2) * sizeof(int));
    while ((n = read(fd, buf, sizeof(buf))) > 0)
    {
        for (i = 0; i < n; i++)
        {
            char c = buf[i];
            if (!inStr && activeVarCount == 0)
            {
                // Check if c is a character in a valid C identifier
                if ((48 <= c && c <= 57) || (65 <= c && c <= 90) || (97 <= c && c <= 122) || (c == '_'))
                {
                    for (j = 0; j < argc - 2; j++)
                    {
                        //  Check if c matches the first character in arg j
                        if (c == vars[j][0])
                        {
                            inVars[j] = 1;
                            // The first charcter in var[j] has been verified
                            varPositions[j] = 0;
                            activeVarCount++;
                        }
                    }
                    if (activeVarCount == 0)
                    {
                        inStr = 1;
                        printf(1, "%c", c);
                    }
                }
                else
                    printf(1, "%c", c);
            }
            else if (activeVarCount > 0)
            {
                for (j = 0; j < argc - 2; j++)
                    if (inVars[j])
                    {
                        inVars[j] = checkVar(&(varPositions[j]), &activeVarCount, c, vars[j], vals[j], inVars, argc - 2);
                        if (!inVars[j] && activeVarCount == 0)
                            printf(1, "%c", c);
                    }
            }
            else
            {
                if ((48 > c || c > 57) && (65 > c || c > 90) && (97 > c || c > 122))
                    inStr = 0;
                printf(1, "%c", c);
            }
        }
    }
    // Check if partial variable was missed at the end.
    if (activeVarCount > 0)
    {
        for (i = 0; i < argc - 2; i++)
        {
            if (inVars[i])
                inVars[i] = checkVar(&(varPositions[i]), &activeVarCount, '\0', vars[i], vals[i], inVars, argc - 2);
        }
    }
    free(inVars);
    free(varPositions);
}
int main(int argc, char *argv[])
{
    int i, fd;
    int varCount = argc - 2;
    char **vars = malloc((argc - 2) * sizeof(char *));
    char **vals = malloc((argc - 2) * sizeof(char *));
    if (argc == 1)
    {
        printf(1, "No input provided for preprocessing.");
        exit();
    }
    if ((fd = open(argv[1], 0)) < 0)
    {
        printf(1, "preprocess: cannot open%s\n", argv[1]);
        exit();
    }
    if (argc > 2)
        organizeArgs(argc, argv, vars, vals);
    commandPreprocess(fd, argc, vars, vals);
    close(fd);
    printf(1, "\n");
    for (i = 0; i < varCount; i++)
    {
        free(vars[i]);
        free(vals[i]);
    }
    free(vars);
    free(vals);
    exit();
}