#include <utility>

#include <utility>

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVMRuntimeError.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abutok <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/31 14:55:01 by abutok            #+#    #+#             */
/*   Updated: 2019/07/31 16:19:59 by abutok           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AVMRuntimeError.hpp"

AVMRuntimeError::AVMRuntimeError(): _message("") {}

AVMRuntimeError::AVMRuntimeError(std::string message): _message(std::move(message)) {}

AVMRuntimeError::AVMRuntimeError(const AVMRuntimeError &ex) noexcept: _message(ex._message){
}

AVMRuntimeError &AVMRuntimeError::operator=(const AVMRuntimeError &) {
	return *this;
}

AVMRuntimeError::~AVMRuntimeError() = default;

const char *AVMRuntimeError::what() const noexcept{
	return this->_message.c_str();
}
