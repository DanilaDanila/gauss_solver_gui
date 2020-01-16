#include "tools.hpp"

#include <stack>
#include <utility>

template<class T>
std::string as_slau(const Vector<Vector<T>> &v)
{
    std::ostringstream out;

    for(size_t i=0; i<v.size(); ++i)
    {
        out<<v[i][0]<<"*x1";
        for(size_t j=1; j<v[0].size()-1; ++j)
            out<<" "<<((v[i][j]>0)?('+'):('-'))<<std::abs(v[i][j])<<"*x"<<j+1;
        out<<" = "<<v[i][v[i].size()-1]<<"\n";
    }

    return out.str();
}

template<class T>
std::string as_matrix(const Vector<Vector<T>> &v)
{
    std::ostringstream out;

    if(v.size() == 1)
    {
        out<<"(";

        for(size_t j=0; j<v[0].size(); ++j)
            out<<std::setw(3)<<v[0][j];

        out<<" )";

        return out.str();
    }

    for(size_t i=0; i<v.size(); ++i)
    {
        if(i == 0)
            out << "/";
        else if (i == v.size() - 1)
            out << "\\";
        else
            out << "|";

        for(size_t j=0; j<v[0].size(); ++j)
            out<<std::setw(3)<<v[i][j];

        if(i == 0)
            out << "  \\";
        else if (i == v.size() - 1)
            out << "  /";
        else
            out << "  |";

        out<<"\n";
    }

    return out.str();
}

template<class T>
std::string as_extended_matrix(const Vector<Vector<T>> &v)
{
    std::ostringstream out;

    for(size_t i=0; i<v.size(); ++i)
    {
        if(i == 0)
            out << "/";
        else if (i == v.size() - 1)
            out << "\\";
        else
            out << "|";

        for(size_t j=0; j<v[0].size()-1; ++j)
            out<<std::setw(3)<<v[i][j];
        out<<" |"<<std::setw(3)<<v[i][v[0].size()-1];

        if(i == 0)
            out << "  \\";
        else if (i == v.size() - 1)
            out << "  /";
        else
            out << "  |";

        out<<"\n";
    }

    return out.str();
}

template<class T>
void slau_next_step(Vector<Vector<T>> &v, const size_t &step)
{
    for(int r=0; r<v.size(); ++r)
    {
        int i;
        for(i=0; (i<v[r].size()) && (v[r][i] == 0); ++i);
        if(v[r][i] < 0) v[r] *= -1;
    }

    std::sort(v.begin()+step, v.end(), [=](auto x, auto y){return std::abs(x[step]) > std::abs(y[step]);});

    std::cerr<<"After sort:\n";
    std::cerr<<as_extended_matrix(v)<<"\n";

    std::cerr<<"("<<step+1<<" , "<<step<<")\n";

    size_t row = step+1;
    for(; row < v.size(); ++row)
    {
        std::cerr<<"v at ("<<row<<" , "<<step<<") = "<<v[row][step]<<"\n";
        if(v[row][step] != 0)
        {
            std::cerr<<"PROCESS:\n";
            std::cerr<<as_extended_matrix(v)<<"\n";

            if(v[step][step] < 0)
            {
                v[step] *= -1;
                std::cerr<<as_extended_matrix(v)<<"\n";
            }

            T least_of = lcm(std::abs(v[step][step]), std::abs(v[row][step]));
            T zero_mul = least_of / v[step][step];
            T row_mul = least_of / v[row][step];

            std::cerr<<step<<" must be mul by "<<zero_mul<<"\n";
            std::cerr<<row<<" must be mul by "<<row_mul<<"\n\n";

            v[step] *= zero_mul;
            v[row] *= row_mul;
            std::cerr<<as_extended_matrix(v)<<"\n";
            v[row] -= v[step];
            std::cerr<<as_extended_matrix(v)<<"\n";
            v[step] /= zero_mul;
            std::cerr<<as_extended_matrix(v)<<"\n";

            T div = T(1);
            for(size_t i=1; i<v[row].size(); ++i)
                if(v[row][i] != 0)
                {
                    div = std::abs(v[row][i]);
                    break;
                }

            for(size_t i=2; i<v[row].size(); ++i)
                if(v[row][i] != 0)
                    div = gcd(div, std::abs(v[row][i]));

            std::cerr<<row<<" can be divided by "<<div<<"\n\n";
            v[row] /= div;
            std::cerr<<as_extended_matrix(v);
            std::cerr<<"DONE!\n";
        }
        std::cerr<<"v at ("<<row<<" , "<<step<<") = "<<v[row][step]<<"\n";
    }
    std::cerr<<"RETURN----------------------------\n";
}

template<class T>
size_t triangle_matrix_rank(const Vector<Vector<T>> &v)
{
    size_t result = 0;

    for(size_t i=0; i<v.size(); ++i)
    {
        T abs(0);
        for(size_t j=0; j<v[i].size(); ++j)
            abs += std::abs(v[i][j]);

        if(abs != T(0))
            ++result;
    }

    return result;
}

template<class T>
const Vector<Vector<T>> transpose(const Vector<Vector<T>> &v)
{
    Vector<Vector<T>> result;

    for(size_t i=0; i<v[0].size(); ++i)
    {
        Vector<T> row;
        for(size_t j=0; j<v.size(); ++j)
            row.append(v[j][i]);
        result.append(row);
    }

    return result;
}

template<class T>
const Vector<Vector<T>> triangulate_foo(const Vector<Vector<T>> &sslau)
{
    Vector<Vector<int>> slau = sslau;

    for(size_t i=0; i<slau[0].size(); ++i)
        slau_next_step(slau, i);

    return slau;
}

template<class T>
const int findLineWithSameLenAs(const Vector<Vector<T>> &sslau, unsigned line_num)
{
    unsigned befor_zeros_count = 0;
    for(unsigned i=0; (i<sslau[line_num].size()) && (sslau[line_num][i] == 0); ++i)
        ++befor_zeros_count;

    int result = -1;

    for(unsigned i=line_num+1; i<sslau.size(); ++i)
    {
        unsigned cur_befor_zeros_count = 0;
        for(unsigned j=0; (j<sslau[i].size()) && (sslau[i][j] == 0); ++j)
            ++cur_befor_zeros_count;

        if(befor_zeros_count == cur_befor_zeros_count)
        {
            result = i;
            break;
        }
    }

    return result;
}

template<class T>
const Vector<Vector<T>> triangulate(const Vector<Vector<T>> &sslau)
{
    Vector<Vector<T>> slau(sslau);

    std::cerr<<as_extended_matrix(slau)<<"\n\n";

    for(unsigned row = 0; row<slau.size()-1; ++row)
    {
        std::sort(slau.begin()+row, slau.end(),
            [](auto x, auto y) {
                unsigned x_num = 0;
                while(x_num < x.size() && x[x_num] == 0) ++x_num;

                unsigned y_num = 0;
                while(y_num < y.size() && y[y_num] == 0) ++y_num;

                if(x._at(x_num) == 0)
                    return false;

                if(y._at(y_num) == 0)
                    return true;

                return x._at(x_num)*x._at(x_num) < y._at(y_num)*y._at(y_num);
            });

        int line;
        while(line = findLineWithSameLenAs(slau, row), line != -1)
        {
            unsigned row_num = 0;
            while(row_num < slau[row].size() && slau[row][row_num] == 0) ++row_num;
            if(slau[row][row_num] < 0)
                slau[row] *= -1;

            unsigned line_num = 0;
            while(line_num < slau[line].size() && slau[line][line_num] == 0) ++line_num;
                if(slau[line][line_num] < 0)
                    slau[line] *= -1;

            std::cerr<<"Cur row = "<<row<<"\nCur line = "<<line<<"\n";
            std::cerr<<as_extended_matrix(slau)<<"\n";

            T least_of = lcm(slau[row][row_num], slau[line][line_num]);
            T row_mul = least_of / slau[row][row_num];
            T line_mul = least_of / slau[line][line_num];

            std::cerr<<"Least of("<<slau[row][row_num]<<" & "<<slau[line][line_num]<<") = "<<least_of<<"\n";

            slau[row] *= row_mul;
            slau[line] *= line_mul;

            std::cerr<<"After mul:\n"<<as_extended_matrix(slau)<<"\n";

            slau[line] -= slau[row];

            std::cerr<<"After sub:\n"<<as_extended_matrix(slau)<<"\n";


            T div = T(1);
            for(size_t i=1; i<slau[line].size(); ++i)
                if(slau[line][i] != 0)
                {
                    div = std::abs(slau[line][i]);
                    break;
                }

            for(size_t i=2; i<slau[line].size(); ++i)
                if(slau[line][i] != 0)
                    div = gcd(div, std::abs(slau[line][i]));

            std::cerr<<"Line div = "<<div<<"\n";
            slau[line] /= div;
            slau[row] /= row_mul;

            std::cerr<<"After div:\n"<<as_extended_matrix(slau)<<"\n\tNEXT STEP\n";
        }
    }

    std::cerr<<as_extended_matrix(slau)<<"\n";

    return slau;
}

const Vector<Vector<Fract>> get_solution(const Vector<Vector<int>> &sslau)
{
    Vector<Vector<int>> slau = triangulate(sslau);

    bool is_no_solution = false;

    for(unsigned row = 0; row<slau.size(); ++row)
    {
        unsigned ssum = 0;
        for(unsigned i=0; i<slau[row].size()-1; ++i)
            ssum += slau[row][i] * slau[row][i];

        if((ssum == 0) && (slau[row][slau[row].size()-1] != 0))
        {
            is_no_solution = true;
            break;
        }
    }

    if(is_no_solution)
    {
        return Vector<Vector<Fract>>();
    }

    size_t rank = triangle_matrix_rank(slau);

    size_t vars_count = slau[0].size()-1;
    size_t fixed_vars_count = vars_count - rank;
    size_t vec_dim = fixed_vars_count + 1;

    Vector<Vector<Fract>> vars(vars_count);

    for(size_t i=0; i<vars_count; ++i)
        for(size_t j=0; j<fixed_vars_count+1; ++j)
            vars[i].append(0);

    if(fixed_vars_count == 0)
    {
        Vector<Fract> e;
        e.append(1);
    
        for(long row = vars_count-1-fixed_vars_count; row>=0; --row)
        {
            Vector<Fract> sum;
            for(size_t i=0; i<vec_dim; ++i)
                sum.append(0);
    
            for(size_t var_n = row+1; var_n<vars_count; ++var_n)
                sum += slau[row][var_n] * vars[var_n];
    
            vars[row] = (e*slau[row][vars_count] - sum) / Fract(slau[row][row]);
        }
    }
    else
    {
        std::stack<std::pair<unsigned, unsigned>> swap_stack;
        for(unsigned i=0; i<slau.size() && i<slau[0].size(); ++i)
            if(slau[i][i] == 0)
            {
                unsigned sum = 0;
                for(unsigned j=0; j<slau[0].size()-1; ++j)
                    sum += slau[i][j] * slau[i][j];

                if(sum == 0)
                    break;

                unsigned non_zero;
                for(non_zero = 0; slau[i][non_zero] == 0; ++non_zero);

                Vector<Vector<int>> new_slau = transpose(slau);

                Vector<int> tmp = new_slau[i];
                new_slau[i] = new_slau[non_zero];
                new_slau[non_zero] = tmp;

                slau = transpose(new_slau);
                swap_stack.push(std::pair<unsigned, unsigned>(i, non_zero));
                std::cerr<<"Swap cols: "<<i<<" "<<non_zero<<"\n"
                         <<as_extended_matrix(slau)<<"\n";
            }


        for(size_t i=0; i<fixed_vars_count; ++i)
            vars[vars_count-1-i][fixed_vars_count-i] = 1;

        Vector<Fract> e;
        e.append(1);
        for(size_t i=1; i<vec_dim; ++i)
            e.append(0);
    
        for(long row = vars_count-1-fixed_vars_count; row>=0; --row)
        {
            Vector<Fract> sum;
            for(size_t i=0; i<vec_dim; ++i)
                sum.append(0);
    
            for(size_t var_n = row+1; var_n<vars_count; ++var_n)
                sum += slau[row][var_n] * vars[var_n];
    
            vars[row] = (e*slau[row][vars_count] - sum) / Fract(slau[row][row]);
        }

        std::cerr<<"Result before swap:\n"
                 <<as_matrix(vars)<<"\n";

        for(unsigned i=0; i<swap_stack.size(); ++i)
        {
            std::pair<unsigned, unsigned> swap_pair = swap_stack.top();
            swap_stack.pop();

            Vector<Fract> tmp = vars[swap_pair.first];
            vars[swap_pair.first] = vars[swap_pair.second];
            vars[swap_pair.second] = tmp;
        }
    }

    std::cerr<<"DONE\n";

	return vars;
}
