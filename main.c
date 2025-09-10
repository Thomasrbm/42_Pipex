/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:05:51 by throbert          #+#    #+#             */
/*   Updated: 2025/09/11 01:07:14 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "pipex.h"

char *get_path(char **env)
{
    int i;

    i = 0;
    while (env[i] != NULL)
    {
        ft_strcmp(); 
    }
    
    
}

int	main(int ac, char **av, char **env)
{
	// int	pipefd[2];

	(void)av;
	if (ac != 5)
	{
		ft_putstr_fd("Wrong number of arguments", 2);
		return (1);
	}
    get_path(env);
    // for (int i = 0; env[i] != NULL; i++)
    //     printf("%s\n", env[i]);
    char *cmd[] = {"ls", "-l", NULL}; // doit etre NULL terminated 
    execve("/bin/ls", cmd, env);
	return (0);
}

// gent env pour avoir apres le PATH=

// on split avec :

// on fait access avec les chemin en add la commande (ls par ex) dans chaque 

// si c est bon on return le dossier ou c est.

// get path  pour recupere les differe dossier ou sont stocker les binaires
// check si ya le chemin absolu de la commande dans chaque fichier 

// si trouver exec dessus.




// fils ecrit dans le pipe 


// try les exec 

// exec le pipe 0 avec pere puis lire avec 1 avec fils
// 
// exec suite dans fils