var timeout;

function ui_solve_letters() {
    var elem = document.getElementById('letters');
    var top = elem.offsetTop;
    while (elem = elem.offsetParent)
        top += elem.offsetTop;
    window.scrollTo(0, top);

    var letters = document.getElementById('letters').value;

    clearTimeout(timeout);
    if (letters.length > 5) {
        /* wait 5 seconds, and if no more letters come post the event */
        timeout = setTimeout(function() {
            _gaq.push(['_trackEvent', 'solve', 'letters', letters.toLowerCase()]);
        }, 5000);
    }

    var result = [];

    solve_letters(letters.toLowerCase(), function(word, c) { result.push([word, c]); });

    result.sort(function(a, b) {
        if (b[0].length != a[0].length)
            return b[0].length - a[0].length;
        else
            return b[1] - a[1];
    });

    document.getElementById('letters-answer').innerHTML = result.map(function(a) { return a[0]; }).join("\n");
}

function ui_reset_letters() {
    document.getElementById('letters').value = '';

    ui_solve_letters();
}

function _ui_solve_numbers(trickshot) {
    var numbers = [];

    for (var i = 1; i <= 6; i++) {
        var x = parseInt(document.getElementById('num'+i).value, 10);

        if (!isNaN(x))
            numbers.push(x);
    }

    var target = parseInt(document.getElementById('target').value, 10);

    if (numbers.length == 6)
        _gaq.push(['_trackEvent', 'solve', trickshot ? 'trickshot' : 'numbers', numbers.join(',')+','+target]);

    if (isNaN(target)) {
        document.getElementById('numbers-answer').innerHTML = 'Invalid target';
        return;
    }

    if (numbers.length < 2) {
        document.getElementById('numbers-answer').innerHTML = 'Not enough numbers';
        return;
    }

    document.getElementById('numbers-answer').innerHTML = solve_numbers(numbers, target, trickshot);
}

function ui_solve_numbers() {
    _ui_solve_numbers(false);
}

function ui_solve_trickshot() {
    _ui_solve_numbers(true);
}

function ui_reset_numbers() {
    for (var i = 1; i <= 6; i++) {
        document.getElementById('num'+i).value = '';
    }
    document.getElementById('target').value = '';

    ui_solve_numbers();
}

document.write("<script src=\"dictionary.js\"></script>");
