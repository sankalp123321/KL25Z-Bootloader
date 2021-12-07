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
 * @brief This function executes when the "author"
 *        command is called. Prints out the autor's name.
 * @param argc number of arguments
 * @param argv arguments
 */
void CmdCentre_Erase(int argc, char* argv[]);

/**
 * @brief This function executes when the dump
 *        command is called.
 * @param argc number of arguments
 * @param argv arguments
 */
void CmdCentre_Program(int argc, char* argv[]);

/**
 *
 * @param argc
 * @param argv
 */
void CmdCentre_BootApplication(int argc, char* argv[]);

/**
 * @brief This function executes when the info
 *        command is called. It prints out the
 *        following:
 *        - Version
 *        - Hostname
 *        - build Date
 *        - git commit version
 * @param argc number of arguments
 * @param argv arguments
 */
void CmdCentre_Info(int argc, char* argv[]);
#endif /* CMDCENTRE_CMDCENTRE_H_ */
