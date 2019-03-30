#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator>
#include <iomanip>

enum cell_type {
	NUMBER, TEXT
};

struct typed_cell {
	std::string value;
	cell_type type;
};

typed_cell val(std::string s) {
	return{ s, TEXT };
}

typed_cell val(double d) {
	return{ std::to_string(d), NUMBER };
}

typedef std::vector<typed_cell> schema_cells;

typedef std::string cell;

typedef std::vector<cell> row;

typedef std::vector<row> matrix;

class data_table_class {
	schema_cells schema;
	matrix rows;
public:
	data_table_class(schema_cells schema) : schema(schema) {}

	void add_row(std::vector<typed_cell> typed_row) {
		row r;
		for (typed_cell cell : typed_row) {
			r.push_back(cell.value);
		}
		rows.push_back(r);
	}

	void print() {
		using namespace std;
		string typenames[] = { "NUMBER", "TEXT" };
		for (auto column : schema) {
			cout << left << setw(25) << setfill(' ') << (column.value + "[" + typenames[column.type] + "]");
		}

		for (auto row : rows) {
			cout << endl;
			for (auto cell : row) {
				cout << left << setw(25) << setfill(' ') << cell;
			}
		}
		cout << endl;
	}

	data_table_class join(data_table_class other) {
		data_table_class thiscopy = *this;
		data_table_class result = schema;

		std::sort(thiscopy.rows.begin(), thiscopy.rows.end());
		std::sort(other.rows.begin(), other.rows.end());

		std::set_union(
			thiscopy.rows.begin(), thiscopy.rows.end(),
			other.rows.begin(), other.rows.end(),
			std::back_inserter(result.rows)
		);

		return result;
	}

	data_table_class difference(data_table_class other) {
		data_table_class thiscopy = *this;
		data_table_class result = schema;

		std::sort(thiscopy.rows.begin(), thiscopy.rows.end());
		std::sort(other.rows.begin(), other.rows.end());

		std::set_difference(
			thiscopy.rows.begin(), thiscopy.rows.end(),
			other.rows.begin(), other.rows.end(),
			std::back_inserter(result.rows)
		);

		return result;
	}

	data_table_class intersection(data_table_class other) {
		data_table_class thiscopy = *this;
		data_table_class result = schema;

		std::sort(thiscopy.rows.begin(), thiscopy.rows.end());
		std::sort(other.rows.begin(), other.rows.end());

		std::set_intersection(
			thiscopy.rows.begin(), thiscopy.rows.end(),
			other.rows.begin(), other.rows.end(),
			std::back_inserter(result.rows)
		);

		return result;
	}
};

int main() {
	using namespace std;

	schema_cells schema = {
		{ "Molecule", TEXT },
		{ "Solubility", NUMBER },
		{ "Molecular Weight", NUMBER }
	};

	data_table_class drugs1 = schema;
	drugs1.add_row({ val("Paracetamol"), val(4.97), val(151) });
	drugs1.add_row({ val("Caffeine"), val(5.05), val(194) });

	cout << "A:" << endl;
	drugs1.print();
	cout << endl;

	data_table_class drugs2 = schema;
	drugs2.add_row({ val("Paracetamol"), val(4.97), val(151) });
	drugs2.add_row({ val("Indomethacin"), val(0.4), val(358) });
	drugs2.add_row({ val("Trimethoprim"), val(3.14), val(290) });

	cout << "B:" << endl;
	drugs2.print();
	cout << endl;

	cout << "A.join(B):" << endl;
	drugs1.join(drugs2).print();
	cout << endl;

	cout << "A.difference(B):" << endl;
	drugs1.difference(drugs2).print();
	cout << endl;

	cout << "B.difference(A):" << endl;
	drugs2.difference(drugs1).print();
	cout << endl;

	cout << "A.intersection(B):" << endl;
	drugs1.intersection(drugs2).print();
	cout << endl;

	system("PAUSE");
}