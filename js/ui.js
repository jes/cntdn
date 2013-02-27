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

    var r = solve_numbers(numbers, target);

    if (r === false) {
        document.getElementById('numbers-answer').value = 'No solution';
    } else {
        var result = r[0];
        var explanation = '';

        if (result != target)
            explanation = '\n(off by ' + Math.abs(target - result) + ')';

        if (r[1] != false) {
            var text = r[1].map(
                function(a) {
                    return a[0] + " " + a[1] + " " + a[2] + " = " + a[3];
                }
            ).join("\n");
        } else {
            text = target + " = " + target;
        }

        document.getElementById('numbers-answer').value = text+explanation;
    }
}
