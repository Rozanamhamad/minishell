/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 22:30:56 by ral-moha          #+#    #+#             */
/*   Updated: 2025/08/05 22:55:01 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// The one allowed global variable
int g_signal_exit_status = 0;

// Signal handler for SIGINT (Ctrl-C) before command execution
void ft_sigint_handler_beforecmd(int sig)
{
    g_signal_exit_status = sig;
    rl_replace_line("", 0);
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_redisplay();
}

// Signal handler for SIGINT (Ctrl-C) during command execution
void ft_sigint_handler_incmd(int sig)
{
    g_signal_exit_status = sig;
    rl_replace_line("", 1);
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_redisplay();
    rl_done = 1;
}

// Signal handler for SIGINT (Ctrl-C) during heredoc
void ft_sigint_heredoc(int sig)
{
    g_signal_exit_status = sig;
    write(STDOUT_FILENO, "\n", 1);
    exit(130);
}

// Signal handler for SIGQUIT (Ctrl-\)
void handle_sigquit(int sig)
{
    g_signal_exit_status = sig;
}

// Set up signals for interactive mode
void prep_signals(void)
{
    signal(SIGINT, ft_sigint_handler_beforecmd);
    signal(SIGQUIT, handle_sigquit);
}

// Check signals in main loop
void ft_check_signal(t_myenv *my_env)
{
    if (g_signal_exit_status == SIGINT)
    {
        my_env->exit_code = 130;
        g_signal_exit_status = 0;
    }
}

// Check exit signals (used during execution)
void check_exit_signals(t_ast_node *node, t_myenv *my_env)
{
    if (g_signal_exit_status == SIGINT)
    {
        my_env->exit_code = 130;
        g_signal_exit_status = 0;
        exit(my_env->exit_code);
    }
    if (g_signal_exit_status == SIGQUIT)
    {
        my_env->exit_code = 131;
        g_signal_exit_status = 0;
    }
    if (node->ex_heredoc)
    {
        node->ex_heredoc = 0;
        exit(0);
    }
}