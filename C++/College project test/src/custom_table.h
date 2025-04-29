#ifndef CUSTOM_TABLE_H
#define CUSTOM_TABLE_H

#include <FL/Fl_Table.H>
#include <string>
#include <vector>

class CustomTable : public Fl_Table {
private:
    std::vector<std::vector<std::string>> data;
    int selectedRow;
    
public:
    CustomTable(int x, int y, int w, int h, const char* label = 0);
    
    void addRow(const std::vector<std::string>& row);
    void clear();
    void resize_table(int rows, int cols);
    void set_value(int row, int col, const char* value);
    
    int getSelectedRow() const { return selectedRow; }
    int getItemId(int row) const;
    int getQuantity(int row) const;
    std::string getItemName(int row) const;
    double getItemPrice(int row) const;
    
protected:
    void draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) override;
    int handle(int event) override;
};

#endif // CUSTOM_TABLE_H 