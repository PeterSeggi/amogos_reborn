#ifndef PHYLO_H
#define PHYLO_H

/**
*@brief     Starts the phylo.
*@param[in] argc Amount of parameters.
*@param[in] argv Pointer to parameters.
*@note      'a' to add a phylo, 'r' to remove. Won't exit on its own.
*/
void phylo_command(int argc, char **argv);

#endif