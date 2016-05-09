#include "sdf_data.hpp"
#include <iostream>

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
        os << "(" << val.min() << ":" << val.typ() << ":" << val.max() << ")";
        return os;
    }
    bool operator==(const RealTriple& lhs, const RealTriple& rhs) {
        return lhs.min() == rhs.min() && lhs.typ() == rhs.typ() && lhs.max() == rhs.max();
    }

} //sdfparse
