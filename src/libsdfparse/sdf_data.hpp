#pragma once

#include <string>
#include <vector>
#include <limits>
#include <iosfwd>
#include <cassert>

#include "sdf_data_fwd.hpp"

//The classes defined in this file correspond (almost directly) to the 
//structures in the SDF file.
//
//The key exception is that for simplicity some basic structures (e.g. CELLTYPE, INSTANCE)
//have been folded into thier parents (e.g. CELL) as attributes.

namespace sdfparse {

    //A collection of three float values (e.g. "(1:2:3)" in SDF, corresponding to
    //the min:typ:max values).
    class RealTriple {
        public:
            RealTriple()
                : min_(std::numeric_limits<double>::quiet_NaN())
                , typ_(std::numeric_limits<double>::quiet_NaN())
                , max_(std::numeric_limits<double>::quiet_NaN())
                {}
            RealTriple(double new_min, double new_typ, double new_max)
                : min_(new_min)
                , typ_(new_typ)
                , max_(new_max)
                {}

            RealTriple(double new_min, double new_max)
                : min_(new_min)
                , typ_(new_max)
                , max_(new_max)
                {}

            double min() const { return min_; }
            double max() const { return max_; }
            double typ() const { return typ_; }

            void print(std::ostream& os, int depth=0) const;

        private:
            double min_;
            double typ_;
            double max_;
    };
    std::ostream& operator<<(std::ostream& os, const RealTriple& val);
    bool operator==(const RealTriple& lhs, const RealTriple& rhs);

    enum class PortCondition {
        POSEDGE,
        NEGEDGE,
        NONE
    };
    std::ostream& operator<<(std::ostream& os, const PortCondition& val);

    class PortSpec {
        public:
            PortSpec() = default;
            PortSpec(std::string port_name, PortCondition port_condition)
                : port_(port_name)
                , condition_(port_condition)
                {}

            std::string port() const { return port_; }
            PortCondition condition() const { return condition_; }

        private:
            std::string port_;
            PortCondition condition_;
    };
    std::ostream& operator<<(std::ostream& os, const PortSpec& val);


    //An IOPATH delcaration
    //
    //Specifies the delay from input() to output() for both rise() and fall() 
    //transitions
    class Iopath {
        public:
            Iopath() = default;
            Iopath(PortSpec new_input, PortSpec new_output, RealTriple new_rise, RealTriple new_fall)
                : input_(new_input)
                , output_(new_output)
                , rise_(new_rise)
                , fall_(new_fall)
                {}

            const PortSpec& input() const { return input_; }
            const PortSpec& output() const { return output_; }
            const RealTriple& rise() const { return rise_; }
            const RealTriple& fall() const { return fall_; }

            void print(std::ostream& os, int depth=0) const;
        private:
            PortSpec input_;
            PortSpec output_;
            RealTriple rise_;
            RealTriple fall_;
    };

    class Timing {
        public:
            Timing() = default;
            Timing(PortSpec clock_spec, PortSpec port_spec, RealTriple value, const std::string& type)
                : clock_(clock_spec)
                , port_(port_spec)
                , t_(value)
                , type_(type)
                {}

            const PortSpec& clock() const { return clock_; }
            const PortSpec& port() const { return port_; }
            RealTriple t() const { return t_; }
            const std::string& type() const { return type_; }

            void print(std::ostream& os, int depth=0) const;
        private:
            PortSpec clock_;
            PortSpec port_;
            RealTriple t_;
            std::string type_;
    };

    class Setup: public Timing {
        public:
            Setup() = default;
            Setup(PortSpec clock_spec, PortSpec port_spec, RealTriple value) :
                Timing(clock_spec, port_spec, value, "SETUP")
            {}
    };

    class Hold: public Timing {
        public:
            Hold() = default;
            Hold(PortSpec clock_spec, PortSpec port_spec, RealTriple value) :
                Timing(clock_spec, port_spec, value, "HOLD")
            {}
    };

    class Recovery: public Timing {
        public:
            Recovery() = default;
            Recovery(PortSpec clock_spec, PortSpec port_spec, RealTriple value) :
                Timing(clock_spec, port_spec, value, "RECOVERY")
            {}
    };

    class Removal: public Timing {
        public:
            Removal() = default;
            Removal(PortSpec clock_spec, PortSpec port_spec, RealTriple value) :
                Timing(clock_spec, port_spec, value, "REMOVAL")
            {}
    };

    class TimingCheck {
        public:
            TimingCheck() = default;
            TimingCheck(std::vector<Timing> timing_checks_vec)
                : timing_checks_(timing_checks_vec)
                {}

            std::vector<Timing> timing() const { return timing_checks_; }

            void print(std::ostream& os, int depth=0) const;
        private:
            std::vector<Timing> timing_checks_;
    };

    //A Delay declaration
    //
    //It consists of a type() and list of iopaths()
    class Delay {
        public:
            enum class Type {
                ABSOLUTE
            };

            Delay() = default;
            Delay(Delay::Type new_type, std::vector<Iopath> new_iopaths)
                : type_(new_type)
                , iopaths_(new_iopaths)
                {}

            Delay::Type type() const { return type_; }
            const std::vector<Iopath>& iopaths() const { return iopaths_; }

            void print(std::ostream& os, int depth=0) const;
        private:
            Delay::Type type_;
            std::vector<Iopath> iopaths_;
    };
    std::ostream& operator<<(std::ostream& os, const Delay::Type& type);

    //A CELL definition
    //
    //It consists of a celltype(), instance() and delay()
    class Cell {
        public:
            Cell() = default;
            Cell(const std::string& new_celltype, const std::string& new_instance, Delay new_delay, TimingCheck timing_check_value)
                : celltype_(new_celltype)
                , instance_(new_instance)
                , delay_(new_delay)
                , timing_check_(timing_check_value)
                {}

                const std::string& celltype() const { return celltype_; }
                const std::string& instance() const { return instance_; }
                const Delay& delay() const { return delay_; }
                const TimingCheck& timing_check() const { return timing_check_; }

                void print(std::ostream& os, int depth=0) const;
        private:
            std::string celltype_;
            std::string instance_;
            Delay delay_;
            TimingCheck timing_check_;
    };

    //A TIMESCALE definition
    //
    //It has a value() and unit()
    class Timescale {
        public:
            Timescale(double new_value=1., const std::string& new_unit="ns")
                : value_(new_value)
                , unit_(new_unit)
                {}

            double value() const { return value_; }
            const std::string& unit() const { return unit_; }

            void print(std::ostream& os, int depth=0) const;
        private:
            double value_;
            std::string unit_;
    };

    //The header (i.e. metadata) for an SDF file.
    //
    //Including the sdfversion(), hierarchical divider() and timescale()
    class Header {
        public:
            Header(const std::string& new_sdfversion="", const std::string& new_divider=".", const Timescale& new_timescale=Timescale(1., "ns"))
                : sdfversion_(new_sdfversion)
                , divider_(new_divider)
                , timescale_(new_timescale)
                {}

            const std::string& sdfversion() const { return sdfversion_; }
            const std::string& design() const { return design_; }
            const std::string& vendor() const { return vendor_; }
            const std::string& program() const { return program_; }
            const std::string& version() const { return version_; }
            const std::string& divider() const { return divider_; }
            const Timescale& timescale() const { return timescale_; }

            void set_design(const std::string& new_design) { design_ = new_design; }
            void set_vendor(const std::string& new_vendor) { vendor_ = new_vendor; }
            void set_program(const std::string& new_program) { program_ = new_program; }
            void set_version(const std::string& new_version) { version_ = new_version; }
            void set_divider(const std::string& new_divider) { divider_ = new_divider; }
            void set_timescale(const Timescale& new_timescale) { timescale_ = new_timescale; }

            void print(std::ostream& os, int depth=0) const;
        private:
            std::string sdfversion_;
            std::string design_;
            std::string vendor_;
            std::string program_;
            std::string version_;
            std::string divider_;
            Timescale timescale_;
    };

    //A DELAYFILE definition
    //
    //This contains all the data included in the parsed SDF file.
    //
    //Organized as a header(), and list of cells().
    class DelayFile {
        public:
            DelayFile(const Header& new_header=Header(), const std::vector<Cell>& new_cells=std::vector<Cell>())
                : header_(new_header)
                , cells_(new_cells)
                {}

            const Header& header() const { return header_; }
            const std::vector<Cell>& cells() const { return cells_; }

            void print(std::ostream& os, int depth=0) const;
        private:
            Header header_;
            std::vector<Cell> cells_;
    };
}
