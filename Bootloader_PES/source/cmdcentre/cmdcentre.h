/**
 * @file cmdcentre.h
 * @brief
 *
 *      This file provides functions for running the cmd engine
 *      and also cmd parser.
 *
 * @author Sankalp Agrawal
 * @date 2021-10-29
 * @version 0.1
 */

#ifndef CMDCENTRE_CMDCENTRE_H_
#define CMDCENTRE_CMDCENTRE_H_

/**
 * @brief This function is responsible for processing
 *        the commands coming in from the word engine.
 * @param cmd_new
 */
void CmdCentre_CommandEngine(char *cmd_new);

/**
 * @brief This function is responsible for processing
 *        the inputs recv'd from the UART
 * @param cmd_new
 * @return
 */
int CmdCentre_WordEngine(char *cmd_new);

/**
 * @brief This function executes when help command
 *        is called. Prints out the command list
 *        in a formatted string.
 * @param argc number of arguments
 * @param argv arguments
 */
void CmdCentre_HelpHandler(int argc, char* argv[]);

/**
 * @brief This function executes when the "erase"
 *        command is called.
 * @param argc number of arguments
 * @param argv arguments
 */
void CmdCentre_Erase(int argc, char* argv[]);

/**
 * @brief This function executes when the "prog"
 *        command is called.
 * @param argc number of arguments
 * @param argv arguments
 */
void CmdCentre_Program(int argc, char* argv[]);

/**
 * @brief Command handler for the "boot" command.
 * 
 * @param argc number of arguments
 * @param argv arguments
 */
void CmdCentre_BootApplication(int argc, char* argv[]);
#endif /* CMDCENTRE_CMDCENTRE_H_ */
