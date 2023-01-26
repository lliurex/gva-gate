// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LLX_GVA_GATE
#define LLX_GVA_GATE

#include <variant.hpp>

#include <cstdio>

#define LLX_GVA_GATE_DB "/tmp/llx-gva-gate.db"

namespace lliurex
{
    class Gate
    {

        public:

        Gate();
        virtual ~Gate();

        bool exists_db();
        edupals::variant::Variant read_db();
        void write_db(edupals::variant::Variant data);

        void create_db();
        void update_db();
        void lock_db_read();
        void lock_db_write();
        void unlock_db();

        edupals::variant::Variant get_groups();

        void test_read();
        void test_write();

        protected:

        FILE* dbase;
    };
}

#endif
