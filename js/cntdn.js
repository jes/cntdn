/* Javascript version of cntdn
 *
 * Countdown game solver
 *
 * James Stanley 2013
 */

function _recurse_solve_letters(letters, node, used_letter, cb, answer) {
    if (node[0])
        cb(answer);

    if (answer.length == letters.length)
        return;

    var done = {};

    for (var i = 0; i < letters.length; i++) {
        var c = letters.charAt(i);

        if (used_letter[i] || done[c])
            continue;

        if (node[c]) {
            used_letter[i] = true;
            done[c] = true;
            _recurse_solve_letters(letters, node[c], used_letter, cb, answer+c);
            used_letter[i] = false;
        }
    }
}

function solve_letters(letters, cb) {
    _recurse_solve_letters(letters, dictionary, {}, cb, '');
}

var OPS = {
    "+": function(n1, n2) { if (n1 < 0 || n2 < 0) return false; return n1+n2; },
    "-": function(n1, n2) { if (n2 >= n1) return false; return n1-n2; },
    "_": function(n2, n1) { if (n2 >= n1) return false; return n1-n2; },
    "*": function(n1, n2) { return n1*n2; },
    "/": function(n1, n2) { if (n2 == 0 || n1%n2 != 0) return false; return n1/n2; },
    "?": function(n2, n1) { if (n2 == 0 || n1%n2 != 0) return false; return n1/n2; },
};

var cache = {};

function _recurse_solve_numbers(numbers, target, levels) {
    if (levels == 0)
        return false;

    var key = numbers + ":" + (levels-1);

    if (key in cache)
        return cache[key];

    for (var i = 0; i < numbers.length-1; i++) {
        var ni = numbers[i];

        if (ni === false)
            continue;

        numbers[i] = false;

        for (var j = i+1; j < numbers.length; j++) {
            var nj = numbers[j];

            if (nj === false)
                continue;

            for (var o in OPS) {
                var r = OPS[o](ni, nj);
                if (r === false)
                    continue;

                if (r == target)
                    return [[ni, o, nj, r]];

                numbers[j] = r;

                var solution = _recurse_solve_numbers(numbers, target, levels-1);
                cache[key] = solution;
                if (solution !== false) {
                    solution.unshift([ni, o, nj, r]);
                    return solution;
                }

                numbers[j] = nj;
            }
        }

        numbers[i] = ni;
    }

    return false;
}

function tidyup_result(result) {
    var mapping = {
        "?": "/", "_": "-"
    };

    for (var i = 0; i < result.length; i++) {
        var subresult = result[i];

        if (subresult[1] in mapping) {
            subresult[1] = mapping[subresult[1]];
            var j = subresult[0];
            subresult[0] = subresult[2];
            subresult[2] = j;
        }
    }

    return result;
}

function solve_numbers(numbers, target) {
    var diff = 0;

    while (diff < 10) {
        /* walk in a range around the target until we have an answer */
        target += diff;
        if (diff > 0)
            diff = -diff;
        else
            diff = -diff + 1;

        cache = {};

        /* see if one of these numbers is the answer */
        for (var i = 0; i < numbers.length; i++)
            if (numbers[i] == target)
                return [target, []];

        /* attempt to solve for this target */
        for (var i = 2; i <= numbers.length; i++) {
            r = _recurse_solve_numbers(numbers, target, i);

            if (r !== false) {
                r = tidyup_result(r);
                cache = {};
                return [target, r];
            }
        }
    }

    cache = {};

    /* no useful answer */
    return false;
}
