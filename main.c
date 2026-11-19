#include "header.h"

char	*alocpy(char *str)
{
	char	*toret;
	int		i;

	if (!str)
		return (NULL);
	toret = malloc(strlen(str) * sizeof(char) + 1);
	if (!toret)
		return (NULL);
	i = 0;
	while (str[i])
	{
		toret[i] = str[i];
		i++;
	}
	toret[i] = '\0';
	return (toret);
}

void	swap_nodes(t_files *node1, t_files *node2)
{
	t_files *temp;

	temp = calloc(1, sizeof(t_files));
	if (!node1 || !node2)
		return ;
	temp->folder = node1->folder;
	temp->to_print = node1->to_print;
	node1->folder = node2->folder;
	node1->to_print = node2->to_print;
	node2->folder = temp->folder;
	node2->to_print = temp->to_print;
	free(temp);
}

void	print_ls(t_files *folders)
{
	t_files	*temp;

	temp = folders;
	while (temp)
	{
		if (temp->to_print == 1)
			printf("%s", temp->folder);	
		if (temp->next && temp->to_print == 1)
			printf("  ");
		temp = temp->next;
	}
	printf("\n");
}

int	check_alpha(char *str1, char *str2)
{
	int i;
	int	j;

	i = 0;
	j = 0;
	while (str1[i] && str2[j])
	{
		if (!isalnum(str1[i]) || !isalnum(str2[j]))
		{
			if (str1[i] == '.')
				i++;
			else
				j++;
			continue ;
		}
		if (toupper(str1[i]) < toupper(str2[j]))
			return (0);
		else if (toupper(str1[i]) > toupper(str2[j]))
			return (1);
		i++;
		j++;
	}
	return (0);
}

void	sort_list(t_files *folders)
{
	t_files *temp;
	t_files *start;

	start = folders;
	while (folders)
	{
		if (folders->next)
		{
			if (check_alpha(folders->folder, folders->next->folder))
			{
				swap_nodes(folders, folders->next);
				folders = start;
			}
			else
				folders = folders->next;
		}
		else
			break ;
	}
}

t_files	*add_node(char *file_name)
{
	t_files *new_node;

	new_node = malloc(sizeof(t_files));
	new_node->folder = alocpy(file_name); 
	new_node->next = NULL;
	if (new_node->folder[0] == '.')
		new_node->to_print = 0;
	else
		new_node->to_print = 1;
	return (new_node);
}

void	free_list(t_files *folders)
{
	t_files *temp;

	while (folders)
	{
		free(folders->folder);
		temp = folders;
		folders = folders->next;
		free(temp);
	}
}

void	simple_ls()
{
	struct dirent *de;
	DIR *dir;
	t_files *folders;
	t_files *head;

	dir = opendir(".");
	de = readdir(dir);
	if (de)
		folders = add_node(de->d_name);
	head = folders;
	while (de)
	{	
		de = readdir(dir);
		if (de)
		{
			folders->next = add_node(de->d_name);
			folders = folders->next;
		}
	}
	folders = head;
	sort_list(folders);
	print_ls(head);
	closedir(dir);
	free_list(head);
}

int main(int ac, char **av)
{
	if (ac > 1)
		return (0);
	else
		simple_ls();
	return (0);
}
