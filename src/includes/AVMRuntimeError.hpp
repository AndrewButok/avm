/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVMRuntimeError.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/31 14:53:19 by abutok            #+#    #+#             */
/*   Updated: 2019/07/31 16:19:59 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_AVMRUNTIMEERROR_HPP
#define AVM_AVMRUNTIMEERROR_HPP


#include <string>

class AVMRuntimeError: public::std::exception {
private:
	const std::string _message;

	AVMRuntimeError &operator=(const AVMRuntimeError&);
public:
	AVMRuntimeError();
	explicit AVMRuntimeError(std::string message);
	AVMRuntimeError(const AVMRuntimeError&) noexcept;

	~AVMRuntimeError() override;

	const char *what() const noexcept override;
};


#endif //AVM_AVMRUNTIMEERROR_HPP
