#pragma once

#include <string>
#include <vector>
#include <limits>
#include <iosfwd>
#include <cassert>

#include "sdf_data_fwd.hpp"

namespace sdfparse {

    class RealTriple {
        public:
            RealTriple()
                : min_(std::numeric_limits<double>::quiet_NaN())
                , typ_(std::numeric_limits<double>::quiet_NaN())
                , max_(std::numeric_limits<double>::quiet_NaN())
                {}
            RealTriple(double min, double typ, double max)
                : min_(min)
                , typ_(typ)
                , max_(max)
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

    class Iopath {
        public:
            Iopath() = default;
            Iopath(const std::string& input, const std::string& output, RealTriple rise, RealTriple fall)
                : input_(input)
                , output_(output)
                , rise_(rise)
                , fall_(fall)
                {}

            const std::string& input() const { return input_; }
            const std::string& output() const { return output_; }
            const RealTriple& rise() const { return rise_; }
            const RealTriple& fall() const { return fall_; }

            void print(std::ostream& os, int depth=0) const;
        private:
            std::string input_;
            std::string output_;
            RealTriple rise_;
            RealTriple fall_;
    };


    class Delay {
        public:
            enum class Type {
                ABSOLUTE
            };

            Delay() = default;
            Delay(Delay::Type type, std::vector<Iopath> iopaths)
                : type_(type)
                , iopaths_(iopaths)
                {}

            Delay::Type type() const { return type_; }
            const std::vector<Iopath>& iopaths() const { return iopaths_; }

            void print(std::ostream& os, int depth=0) const;
        private:
            Delay::Type type_;
            std::vector<Iopath> iopaths_;
    };
    std::ostream& operator<<(std::ostream& os, const Delay::Type& type);

    class Cell {
        public:
            Cell() = default;
            Cell(const std::string& celltype, const std::string& instance, Delay delay)
                : celltype_(celltype)
                , instance_(instance)
                , delay_(delay)
                {}

                const std::string& celltype() const { return celltype_; }
                const std::string& instance() const { return instance_; }
                const Delay& delay() const { return delay_; }

                void print(std::ostream& os, int depth=0) const;
        private:
            std::string celltype_;
            std::string instance_;
            Delay delay_;
    };

    class Timescale {
        public:
            Timescale(double value=1., const std::string& unit="ns")
                : value_(value)
                , unit_(unit)
                {}

            double value() const { return value_; }
            const std::string& unit() const { return unit_; }

            void print(std::ostream& os, int depth=0) const;
        private:
            double value_;
            std::string unit_;
    };

    class Header {
        public:
            Header(const std::string& sdfversion="", const std::string& divider=".", const Timescale& timescale=Timescale(1., "ns"))
                : sdfversion_(sdfversion)
                , divider_(divider)
                , timescale_(timescale)
                {}

            const std::string& sdfversion() const { return sdfversion_; }
            const std::string& divider() const { return divider_; }
            const Timescale& timescale() const { return timescale_; }

            void set_divider(const std::string& divider) { divider_ = divider; }
            void set_timescale(const Timescale& timescale) { timescale_ = timescale; }

            void print(std::ostream& os, int depth=0) const;
        private:
            std::string sdfversion_;
            std::string divider_;
            Timescale timescale_;
    };

    class DelayFile {
        public:
            DelayFile(const Header& header=Header(), const std::vector<Cell>& cells=std::vector<Cell>())
                : header_(header)
                , cells_(cells)
                {}

            const Header& header() const { return header_; }
            const std::vector<Cell>& cells() const { return cells_; }

            void print(std::ostream& os, int depth=0) const;
        private:
            Header header_;
            std::vector<Cell> cells_;
    };
}
