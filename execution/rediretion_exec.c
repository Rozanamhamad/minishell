/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediretion_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 23:47:36 by ral-moha          #+#    #+#             */
/*   Updated: 2025/07/28 23:34:55 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// bool setup_redirections(t_ast_node *node, int *fd_in, int *fd_out, t_myenv *env)
// {
//     if (node->in) {
//         *fd_in = open(node->in, O_RDONLY);
//         if (*fd_in == -1) {
//             perror(node->in);
//             env->exit_code = 1;
//             return false;
//         }
//     }
//     if (node->out) {
//         *fd_out = open(node->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         if (*fd_out == -1) {
//             perror(node->out);
//             env->exit_code = 1;
//             return false;
//         }
//     }
//     if (node->append) {
//         *fd_out = open(node->append, O_WRONLY | O_CREAT | O_APPEND, 0644);
//         if (*fd_out == -1) {
//             perror(node->append);
//             env->exit_code = 1;
//             return false;
//         }
//     }
//     return true;
// }

