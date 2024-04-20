use std::{io, vec::Vec};

fn get_input() -> Vec<Vec<isize>> {
    let stdin = io::stdin();
    println!("No. of rows in matrix: ");
    let mut rows_string = String::new();
    stdin
        .read_line(&mut rows_string)
        .expect("Failed to read line");
    let rows = rows_string
        .trim()
        .to_string()
        .parse::<usize>()
        .expect("Unable to parse number");

    println!("No. of cols in matrix: ");
    let mut col_string = String::new();
    stdin
        .read_line(&mut col_string)
        .expect("Failed to read line");
    let cols = col_string
        .trim()
        .to_string()
        .parse::<usize>()
        .expect("Unable to parse number");
    let mut matrix: Vec<Vec<isize>> = Vec::new();
    println!("Enter the {} elements of the matrix", rows * cols);

    for _ in 0..rows {
        let mut row: Vec<isize> = Vec::new();
        for _ in 0..cols {
            let mut ele_string = String::new();
            stdin
                .read_line(&mut ele_string)
                .expect("Failed to read line");
            row.push(
                ele_string
                    .trim()
                    .to_string()
                    .parse::<isize>()
                    .expect("Unable to parse integer"),
            );
        }
        matrix.push(row);
    }

    return matrix;
}

fn check_toeplitz(matrix: Vec<Vec<isize>>) {
    println!("{:?}", matrix);
    // first col up;
    for j in matrix[0].len() - 2..1 {}
}

fn main() {
    check_toeplitz(get_input());
}
