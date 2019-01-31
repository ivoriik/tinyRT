/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/19 17:35:53 by vbespalk          #+#    #+#             */
/*   Updated: 2018/06/28 12:09:58 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sdl_event.h"

// int		exit_x(void *mlx)
// {
// 	t_mlx *m;

// 	m = (t_mlx *)mlx;
// 	mlx_destroy_image(m->mlx_ptr, m->img_ptr);
// 	sdl_close(m->sdl);
// 	exit(0);
// 	return (0);
// }

// int		hook_keydown(int k, void *mlx)
// {
// 	t_mlx *ptr;

// 	ptr = (t_mlx *)mlx;
// 	if (k == 53)
// 		exit_x(mlx);
// 	if (k == 84 || k == 86 || k == 88 || k == 91 || k == 69 || k == 78)
// 		ptr->selected ? translate(k, &(ptr->selected->tr_pos), 0) : \
// 	translate(k, &(ptr->scene->cam_transl), 1);
// 	if (k == 67 || k == 75)
// 		ptr->selected ? scale(k, &(ptr->selected->tr_siz), 0) : \
// 	scale(k, &(ptr->scene->fov), 1);
// 	if (k == 123 || k == 124 || k == 125 || k == 126 || k == 116 || k == 121)
// 		ptr->selected ? rotate(k, &(ptr->selected->tr_rot)) : \
// 	cam_rotate(k, &(ptr->scene->cam_angles));
// 	if (k == 82)
// 		reset(ptr);
// 	if (k == 87)
// 	{
// 		ptr->selected = NULL;
// 		return (0);
// 	}
// 	put_to_window(ptr);
// 	return (0);
// }

void	on_key_down(SDL_Keycode sum, Uint16 mod, t_env *e)
{
	if (sum == SDLK_w || sum == SDLK_a \
		|| sum == SDLK_s || sum == SDLK_d \
		|| sum == SDLK_LSHIFT || sum == SDLK_LCTRL)
	{
		e->selected ? translate(sum, &(e->selected->tr_pos), 0) : \
	translate(sum, &(e->scene->cam_transl), 1);
	}
	if (sum == SDLK_UP || sum == SDLK_DOWN || \
		sum == SDLK_LEFT || sum == SDLK_RIGHT || \
		sum == SDLK_PAGEDOWN|| sum == SDLK_PAGEUP)
	{
		e->selected ? rotate(sum, &(e->selected->tr_rot)) : \
	cam_rotate(sum, &(e->scene->cam_angles));
	}
	if (sum == SDLK_KP_PLUS || sum == SDLK_KP_MINUS)
	{
		e->selected ? scale(sum, &(e->selected->tr_siz), 0) : \
	scale(sum, &(e->scene->fov), 1);
	}
	if (sum == SDLK_r)
	{
		reset(e);
	}
	return ;
}

// void	on_key_down(SDL_Keycode sum, Uint16 mod, t_env *e)
// {
// 	if (sum == SDL_SCANCODE_KP_8 || sum == SDL_SCANCODE_KP_2 \
// 		|| sum == SDL_SCANCODE_KP_4 || sum == SDL_SCANCODE_KP_6)
// 	{
// 		ptr->selected ? translate(sum, &(e->selected->tr_pos), 0) : \
// 	translate(k, &(e->scene->cam_transl), 1);
// 	}
// 	if (sum == SDL_SCANCODE_UP || sum == SDL_SCANCODE_DOWN || \
// 		sum == SDL_SCANCODE_LEFT || sum == SDL_SCANCODE_RIGHT || \
// 		sum == SDL_SCANCODE_PAGEDOWN|| sum == SDL_SCANCODE_PAGEUP)
// 	{
// 		ptr->selected ? rotate(sum, &(e->selected->tr_rot)) : \
// 	cam_rotate(k, &(e->scene->cam_angles));
// 	}
// 	if (sum == SDL_SCANCODE_KP_PLUS || sum == SDL_SCANCODE_KP_MINUS)
// 	{
// 		ptr->selected ? scale(sum, &(e->selected->tr_siz), 0) : \
// 	scale(k, &(e->scene->fov), 1);
// 	}
// 	if (sum == SDL_SCANCODE_KP_0)
// 	{
// 		e->selected = NULL;
// 		return (0);
// 	}
// 	return ;
// }

void	on_key_up(SDL_Keycode sum, Uint16 mod, t_env *e)
{
	printf("key_up\n");
	return ;
}

void	on_mouse_move(int x, int y, int rel_x, int rel_y, t_env *e, int left, int right, int middle)
{
	printf("mouse_move pos:%d,%d, rel %d,%d left %i, right %i, middle %i\n", x, y, rel_x, rel_y, left, right, middle);
	return;
}

void	on_lbutton_down(int x, int y, t_env *e)
{
	if (e->pix_obj[y * SCR_WID + x])
		e->selected = (e->pix_obj)[y * SCR_WID + x];
	else
		e->selected = NULL;
	// printf("left but_down %d,%d\n", x, y);
}

void	on_rbutton_down(int x, int y, t_env *e)
{
	e->selected = NULL;
	// printf("right but_down %d,%d\n", x, y);
}

void	on_lbutton_up(int x, int y, t_env *e)
{
	// printf("left but_up %d,%d\n", x, y);
}

// int		mouse_hook(int button, int x, int y, void *mlx)
// {
// 	t_mlx *ptr;

// 	ptr = (t_mlx *)mlx;
// 	if (button == 1)
// 	{
// 		if (ptr->pix_obj[y * SCR_WID + x])
// 			ptr->selected = (ptr->pix_obj)[y * SCR_WID + x];
// 		else
// 			ptr->selected = NULL;
// 	}
// 	return (0);
// }
