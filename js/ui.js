function ui_solve_letters() {
    var letters = document.getElementById('letters').value;

    var result = [];

    solve_letters(letters, function(word) { result.push(word); });

    result.sort(function(a, b) { return b.length - a.length; });

    document.getElementById('letters-answer').value = result.join("\n");
}

function ui_solve_numbers() {
    var numbers = [];

    for (var i = 1; i <= 6; i++) {
        var x = parseInt(document.getElementById('num'+i).value, 10);

        if (!isNaN(x))
            numbers.push(x);
    }

    var target = parseInt(document.getElementById('target').value, 10);
    if (isNaN(target)) {
        document.getElementById('numbers-answer').value = 'Invalid target';
        return;
    }

    if (numbers.length == 0) {
        document.getElementById('numbers-answer').value = 'No numbers';
        return;
    }

    document.getElementById('numbers-answer').value = solve_numbers(numbers, target);
}
