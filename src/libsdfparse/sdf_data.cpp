#include "sdf_data.hpp"
#include <iostream>
#include <cmath>

namespace /*anonymous*/ {
    std::string ident(int depth);

    std::string ident(int depth) {
        return std::string(2*depth, ' ');
    }
}
namespace sdfparse {


    void DelayFile::print(std::ostream& os, int depth) const {
        os << ident(depth) << "(DELAYFILE\n";

        header().print(os, depth+1);

        for(auto& cell : cells()) {
            cell.print(os, depth+1);
        }
        os << ident(depth) << ")\n";
        
    }

    void Header::print(std::ostream& os, int depth) const {
        os << ident(depth) << "(SDFVERSION \"" << sdfversion() << "\")\n";
        os << ident(depth) << "(DESIGN \"" << design() << "\")\n";
        os << ident(depth) << "(VENDOR \"" << vendor() << "\")\n";
        os << ident(depth) << "(PROGRAM \"" << program() << "\")\n";
        os << ident(depth) << "(VERSION \"" << version() << "\")\n";
        os << ident(depth) << "(DIVIDER " << divider() << ")\n";
        timescale().print(os, depth);
    }

    void Timescale::print(std::ostream& os, int depth) const {
        os << ident(depth) << "(TIMESCALE " << value() << " " << unit() << ")\n";
    }

    void Cell::print(std::ostream& os, int depth) const {
        os << ident(depth) << "(CELL\n";
        os << ident(depth+1) << "(CELLTYPE \"" << celltype() << "\")\n";
        os << ident(depth+1) << "(INSTANCE " << instance() << ")\n";
        delay().print(os, depth+1);
        timing_check().print(os, depth+1);
        os << ident(depth) << ")\n";
    }

    void Delay::print(std::ostream& os, int depth) const {
        if(!iopaths_.empty()) {
            os << ident(depth) << "(DELAY\n";
            os << ident(depth+1) << "(" << type() << "\n";
            for(auto& iopath : iopaths()) {
                iopath.print(os, depth+2);
            }
            os << ident(depth+1) << ")\n";
            os << ident(depth) << ")\n";
        }
    }

    void Setup::print(std::ostream& os, int depth) const {
        os << ident(depth) << "(SETUP " << port() << " " << clock() << " " << tsu() << ")\n";
    }
    void Hold::print(std::ostream& os, int depth) const {
        os << ident(depth) << "(HOLD " << port() << " " << clock() << " " << thld() << ")\n";
    }

    void TimingCheck::print(std::ostream& os, int depth) const {
        if(!setup().empty() || !hold().empty()) {
            os << ident(depth) << "(TIMINGCHECK\n";
            for(auto& setup_check : setup()) {
                setup_check.print(os, depth+1);
            }
            for(auto& hold_check : hold()) {
                hold_check.print(os, depth+1);
            }
            os << ident(depth) << ")\n";
        }
    }

    std::ostream& operator<<(std::ostream& os, const Delay::Type& type) {
        if(type == Delay::Type::ABSOLUTE) {
            os << "ABSOLUTE";
        } else {
            assert(false);
        }
        return os;
    }

    void Iopath::print(std::ostream& os, int depth) const {
        os << ident(depth) << "(IOPATH " << input() << " " << output() << " " << rise() << " " << fall() << ")\n";
    }

    std::ostream& operator<<(std::ostream& os, const RealTriple& val) {
        if(std::isnan(val.min()) && std::isnan(val.typ()) && std::isnan(val.max())) {
            os << "()";
        } else {
            os << "(" << val.min() << ":" << val.typ() << ":" << val.max() << ")";
        }
        return os;
    }
    bool operator==(const RealTriple& lhs, const RealTriple& rhs) {
        return lhs.min() == rhs.min() && lhs.typ() == rhs.typ() && lhs.max() == rhs.max();
    }

    std::ostream& operator<<(std::ostream& os, const PortSpec& port_spec) {
        if(port_spec.condition() != PortCondition::NONE) {
            os << "(" << port_spec.condition() << " ";
        }
        os << port_spec.port();
        if(port_spec.condition() != PortCondition::NONE) {
            os << ")";
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const PortCondition& val) {
        if(val == PortCondition::POSEDGE) os << "posedge";
        else if (val == PortCondition::NEGEDGE) os << "negedge";
        else if (val == PortCondition::NONE) os << "none";
        else assert(false);
        return os;
    }

} //sdfparse
