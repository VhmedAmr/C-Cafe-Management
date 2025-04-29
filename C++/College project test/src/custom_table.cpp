#include "custom_table.h"
#include <FL/fl_draw.H>

CustomTable::CustomTable(int x, int y, int w, int h, const char* label)
    : Fl_Table(x, y, w, h, label), selectedRow(-1) {
    // Set up the table
    selection_color(FL_YELLOW);
    when(FL_WHEN_RELEASE);
    
    // Set up the columns
    col_header(1);
    col_header_height(25);
    col_resize(1);
    
    // Set up the rows
    row_header(1);
    row_header_width(50);
    row_height_all(25);
    row_resize(0);
}

void CustomTable::addRow(const std::vector<std::string>& row) {
    data.push_back(row);
    rows(data.size());
    cols(row.size());
    redraw();
}

void CustomTable::clear() {
    data.clear();
    rows(0);
    cols(0);
    selectedRow = -1;
    redraw();
}

void CustomTable::resize_table(int rows, int cols) {
    this->rows(rows);
    this->cols(cols);
    redraw();
}

void CustomTable::set_value(int row, int col, const char* value) {
    // Ensure the data vector has enough rows
    while (data.size() <= static_cast<size_t>(row)) {
        data.push_back(std::vector<std::string>());
    }
    
    // Ensure the row has enough columns
    while (data[row].size() <= static_cast<size_t>(col)) {
        data[row].push_back("");
    }
    
    // Set the value
    data[row][col] = value;
    redraw();
}

int CustomTable::getItemId(int row) const {
    if (row >= 0 && row < data.size()) {
        return std::stoi(data[row][0]);
    }
    return -1;
}

int CustomTable::getQuantity(int row) const {
    if (row >= 0 && row < data.size() && data[row].size() >= 4) {
        return std::stoi(data[row][3]);
    }
    return 0;
}

std::string CustomTable::getItemName(int row) const {
    if (row >= 0 && row < data.size() && data[row].size() >= 2) {
        return data[row][1];
    }
    return "";
}

double CustomTable::getItemPrice(int row) const {
    if (row >= 0 && row < data.size() && data[row].size() >= 3) {
        return std::stod(data[row][2]);
    }
    return 0.0;
}

void CustomTable::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H) {
    switch (context) {
        case CONTEXT_CELL: {
            fl_push_clip(X, Y, W, H);
            fl_color(FL_WHITE);
            fl_rectf(X, Y, W, H);
            fl_color(FL_GRAY0);
            fl_rect(X, Y, W, H);
            
            if (R < data.size() && C < data[R].size()) {
                fl_draw(data[R][C].c_str(), X + 2, Y, W, H, FL_ALIGN_LEFT | FL_ALIGN_WRAP);
            }
            
            fl_pop_clip();
            break;
        }
        case CONTEXT_COL_HEADER:
            fl_push_clip(X, Y, W, H);
            fl_color(FL_GRAY0);
            fl_rectf(X, Y, W, H);
            fl_color(FL_WHITE);
            fl_rect(X, Y, W, H);
            fl_color(FL_WHITE);
            fl_draw("Column", X + 2, Y, W, H, FL_ALIGN_LEFT | FL_ALIGN_WRAP);
            fl_pop_clip();
            break;
        case CONTEXT_ROW_HEADER:
            fl_push_clip(X, Y, W, H);
            fl_color(FL_GRAY0);
            fl_rectf(X, Y, W, H);
            fl_color(FL_WHITE);
            fl_rect(X, Y, W, H);
            fl_color(FL_WHITE);
            fl_draw(std::to_string(R + 1).c_str(), X + 2, Y, W, H, FL_ALIGN_LEFT | FL_ALIGN_WRAP);
            fl_pop_clip();
            break;
    }
}

int CustomTable::handle(int event) {
    int ret = Fl_Table::handle(event);
    
    switch (event) {
        case FL_PUSH: {
            int R = callback_row();
            if (R >= 0 && R < data.size()) {
                selectedRow = R;
                redraw();
            }
            break;
        }
    }
    
    return ret;
} 