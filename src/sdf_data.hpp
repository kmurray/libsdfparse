#pragma once
#include <sdf_data_fwd.hpp>

#include <string>
#include <vector>
#include <limits>

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

        private:
            double min_;
            double typ_;
            double max_;
    };

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

        private:
            Delay::Type type_;
            std::vector<Iopath> iopaths_;
    };

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
        private:
            std::string celltype_;
            std::string instance_;
            Delay delay_;
    };

    class Timescale {
        public:
            Timescale(double scale=1., const std::string& unit="ns")
                : scale_(scale)
                , unit_(unit)
                {}

            double scale() { return scale_; }
            const std::string& unit() { return unit_; }
        private:
            double scale_;
            std::string unit_;
    };

    class Header {
        public:
            Header(const std::string& version="", const std::string& hierarchy_divider=".", const Timescale& timescale=Timescale(1., "ns"))
                : version_(version)
                , hierarchy_divider_(hierarchy_divider)
                , timescale_(timescale)
                {}

            const std::string& version() const { return version_; }
            const std::string& hierarchy_divider() const { return hierarchy_divider_; }
            const Timescale& timescale() const { return timescale_; }

            void set_version(const std::string& version) { version_ = version; }
            void set_hierarchy_divider(const std::string& hierarchy_divider) { hierarchy_divider_ = hierarchy_divider; }
            void set_timescale(const Timescale& timescale) { timescale_ = timescale; }
        private:
            std::string version_;
            std::string hierarchy_divider_;
            Timescale timescale_;
    };

    class DelayFile {
        public:
            DelayFile(const Header& header=Header(), const std::vector<Cell>& cells=std::vector<Cell>())
                : header_(header)
                , cells_(cells)
                {}

            const Header& header() { return header_; }
            const std::vector<Cell>& cells() { return cells_; }
        private:
            Header header_;
            std::vector<Cell> cells_;
    };
}
