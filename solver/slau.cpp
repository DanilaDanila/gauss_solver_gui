#include "tools.hpp"

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
    std::sort(v.begin()+step, v.end(), [=](auto x, auto y){return std::abs(x[step]) > std::abs(y[step]);});

    std::cerr<<"After sort:\n";
    std::cerr<<as_extended_matrix(v)<<"\n";

    size_t row = step+1;
    for(; row < v.size(); ++row)
        if(v[row][step] != 0)
        {
            std::cerr<<"PROCESS:\n";
            std::cerr<<as_extended_matrix(v)<<"\n";

            if(v[step][step] < 0)
            {
                v[step] *= -1;
                std::cerr<<as_extended_matrix(v)<<"\n";
            }

            T least_of = lcm(v[step][step], std::abs(v[row][step]));
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
const Vector<Vector<T>> triangulate(const Vector<Vector<T>> &sslau)
{
    Vector<Vector<int>> slau = sslau;

    for(size_t i=0; i<slau.size()-1; ++i)
        slau_next_step(slau, i);

    return slau;
}

const Vector<Vector<Fract>> get_solution(const Vector<Vector<int>> &sslau)
{
    Vector<Vector<int>> slau = triangulate(sslau);

    size_t rank = triangle_matrix_rank(slau);

    size_t vars_count = slau[0].size()-1;
    size_t fixed_vars_count = vars_count - rank;
    size_t vec_dim = fixed_vars_count + 1;
    Vector<Vector<Fract>> vars(vars_count);

    for(size_t i=0; i<vars_count; ++i)
        for(size_t j=0; j<fixed_vars_count+1; ++j)
            vars[i].append(0);

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

	return vars;
}
