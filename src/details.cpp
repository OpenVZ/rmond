/*
 * Copyright (c) 2016-2017, Parallels International GmbH
 * Copyright (c) 2017-2019 Virtuozzo International GmbH. All rights reserved.
 *
 * This file is part of OpenVZ. OpenVZ is free software; you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * Our contact details: Virtuozzo International GmbH, Vordergasse 59, 8200
 * Schaffhausen, Switzerland.
 */

#include "details.h"
#include "handler.h"

namespace Rmond
{
///////////////////////////////////////////////////////////////////////////////
// struct Schema<void>

Oid_type Schema<void>::uuid(Oid_type::value_type luid_)
{
	Oid_type output = Central::product();
	output.push_back(luid_);
	return output;
}

namespace Table
{
namespace Request
{
///////////////////////////////////////////////////////////////////////////////
// struct Details

const char Details::s_NAME[] = TOKEN_PREFIX"backup";

Details::Details(request_type* request_): m_request(request_)
{
}

void Details::backup(netsnmp_variable_list* data_)
{
	if (NULL == m_request)
	{
		snmp_free_varbind(data_);
		snmp_log(LOG_ERR, "backup is impossible\n");

		return cannot(SNMP_ERR_GENERR);
	}
	Netsnmp_Free_List_Data* b = (Netsnmp_Free_List_Data* )&snmp_free_varbind;
	netsnmp_request_add_list_data(m_request,
	      netsnmp_create_data_list(s_NAME, data_, b));
}

netsnmp_variable_list* Details::restore()
{
	netsnmp_variable_list* output = (netsnmp_variable_list* )
		netsnmp_request_get_list_data(m_request, s_NAME);
	if (NULL != output)
		netsnmp_request_remove_list_data(m_request, s_NAME);

	return output;
}

Details::cell_type* Details::cell() const
{
	return netsnmp_extract_table_info(m_request);
}

void Details::cannot(int code_)
{
	netsnmp_request_set_error(m_request, code_);
}

} // namespace Request
} // namespace Table
} // namespace Rmond

