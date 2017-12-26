/*
 * Copyright 2017 WolkAbout Technology s.r.o.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WOLKOPTIONAL_H
#define WOLKOPTIONAL_H

#include <type_traits>

namespace wolkabout
{

template<typename T>
class WolkOptional
{
public:
	WolkOptional() : m_value{}, m_null{true}
    {
        static_assert(std::is_default_constructible<T>::value, "Type doesn't have default constructor");
    }

    WolkOptional(T value) : m_value{value}, m_null{false} {}

    WolkOptional<T>& operator=(T value)
    {
        m_value = value;
        m_null = false;

        return *this;
    }

    operator T() const { return m_value; }

    bool null() const { return m_null; }

private:
    T m_value;
    bool m_null;
};

}

#endif // WOLKOPTIONAL_H