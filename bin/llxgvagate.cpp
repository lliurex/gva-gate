// SPDX-FileCopyrightText: 2022 Enrique M.G. <quiqueiii@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "libllxgvagate.hpp"
#include "hash.hpp"

#include <variant.hpp>
#include <console.hpp>
#include <cmd.hpp>

#include <iostream>
#include <string>

using namespace lliurex;
using namespace lliurex::hash;

using namespace edupals;
using namespace edupals::variant;

using namespace std;

int debug_level = LOG_ERR;

void log(int priority,string message)
{
    if (priority > debug_level ) {
        return;
    }

    if (priority <= LOG_ERR) {
        clog<<console::fg::red<<console::style::bold<<message<<console::reset::all;
    }
    else {
        clog<<message;
    }

    clog.flush();
}

int main(int argc,char* argv[])
{
    cmd::ArgumentParser parser;
    cmd::ParseResult result;

    parser.add_option(cmd::Option('h',"help",cmd::ArgumentType::None));
    parser.add_option(cmd::Option('v',"version",cmd::ArgumentType::None));
    parser.add_option(cmd::Option('d',"debug",cmd::ArgumentType::Required));
    parser.add_option(cmd::Option("verbose",cmd::ArgumentType::None));

    result=parser.parse(argc,argv);

    if (!result.success()) {
        return 1;
    }

    for (cmd::Option o:result.options) {
        if (o.short_name=='d') {
            debug_level = std::stoi(o.value);
        }

        if (o.long_name == "verbose") {
            debug_level = 7;
        }
    }

/*
    for (cmd::Option o:result.options) {
        clog<<"+ "<<o.short_name<<endl;
    }

    for (string s:result.args) {
        clog<<"* "<<s<<endl;
    }
*/

    string cmd;

    if (result.args.size()>1) {
        cmd = result.args[1];
    }

    if (cmd == "create") {
        Gate gate(log);
        if (!gate.exists_db()) {
            gate.create_db();
        }
        else {
            clog<<"database already exists"<<endl;
        }
    }

    if (cmd == "machine-token") {
        Gate gate(log);
        gate.open();
        cout<<gate.machine_token()<<endl;
    }

    if (cmd == "groups") {
        Gate gate(log);
        gate.open();
        Variant groups = gate.get_groups();
        for (int n=0;n<groups.count();n++) {
            cout<<groups[n]["name"].get_string()<<":"<<groups[n]["gid"]<<":";

            Variant members = groups[n]["members"];

            for (size_t n=0;n<members.count();n++) {
                cout<<members[n].get_string();

                if (n<(members.count()-1)) {
                    cout<<",";
                }
            }

            cout<<endl;
        }
    }

    if (cmd == "users") {
        Gate gate(log);
        gate.open();
        Variant users = gate.get_users();
        for (int n=0;n<users.count();n++) {
            Variant passwd = users[n];

            cout<<passwd["name"].get_string()<<":x:";
            cout<<passwd["uid"].get_int32()<<":";
            cout<<passwd["gid"].get_int32()<<":";
            cout<<passwd["gecos"].get_string()<<":";
            cout<<passwd["dir"].get_string()<<":";
            cout<<passwd["shell"].get_string()<<endl;
        }
    }

    if (cmd == "login" and argc>3) {
        Gate gate(log);
        gate.open();
        gate.authenticate(argv[2],argv[3]);
    }

    if (cmd == "test-hash") {
        string input = "quique";
        clog<<"input:"<<input<<endl;
        clog<<lliurex::hash::sha1(input)<<endl;
    }

    return 0;
}
