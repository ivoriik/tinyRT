/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbespalk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 17:19:29 by vbespalk          #+#    #+#             */
/*   Updated: 2018/08/21 16:11:31 by vbespalk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

// void			put_to_window(t_env *ptr)
// {
// 	render(ptr, ptr->scene);
// 	SDL_RenderClear(ptr->sdl->renderer);
// 	SDL_UpdateTexture(ptr->sdl->screen, NULL, ptr->sdl->pixels, SCR_WID * sizeof(Uint32));
// 	SDL_RenderCopy(ptr->sdl->renderer, ptr->sdl->screen, NULL, NULL);
// 	SDL_RenderPresent(ptr->sdl->renderer);
// }

unsigned int	get_rgb(t_sdl *sdl, unsigned int red, unsigned int green, unsigned int blue)
{
	unsigned int color;
	Uint32 col;

	// color = red;
	// color = (color << 8) + green;
	// color = (color << 8) + blue;
	// color = blue;
	// color = (color << 8) + green;
	// color = (color << 8) + red;
	col = SDL_MapRGB(sdl->format, red, green, blue);
	return ((unsigned int)col);
}

void			img_pixel_put(t_env *env, int x, int y, unsigned int color)
{
	Uint32 *data;

	if (!(x >= 0 && x < SCR_WID && y >= 0 && y < SCR_HEI))
		return ;
	data = env->sdl->pixels;
	data += y * (env->sdl->pitch) + x;
	ft_memcpy(data, &color, sizeof(color));
}
