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

var bestdiff;
var bestops;
var bestvalsums;

var OPS = {
    "+": function(n1, n2) { if (n1 < 0 || n2 < 0) return false; return n1+n2; },
    "-": function(n1, n2) { if (n2 >= n1) return false; return n1-n2; },
    "_": function(n2, n1) { if (n2 >= n1) return false; return n1-n2; },
    "*": function(n1, n2) { return n1*n2; },
    "/": function(n1, n2) { if (n2 == 0 || n1%n2 != 0) return false; return n1/n2; },
    "?": function(n2, n1) { if (n2 == 0 || n1%n2 != 0) return false; return n1/n2; },
};

function _recurse_solve_numbers(starti, numbers, target, levels, howto, valsums) {
    if (levels == 0)
        return;

    for (var i = starti; i < numbers.length-1; i++) {
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

                howto.push([ni, o, nj, r]);

                var newvalsums = valsums + r;

                if ((Math.abs(r - target) < Math.abs(bestresult - target))
                        || (Math.abs(r - target) == Math.abs(bestresult - target) && newvalsums < bestvalsums)) {
                    bestresult = r;
                    bestops = howto.slice(0);
                    bestvalsums = newvalsums;
                }

                numbers[j] = r;

                _recurse_solve_numbers(i+1, numbers, target, levels-1, howto, newvalsums);

                howto.pop();

                numbers[j] = nj;
            }
        }

        numbers[i] = ni;
    }
}

function tidyup_result(result) {
    var mapping = {
        "?": "/", "_": "-"
    };

    var dontswap = {
        "/": 1, "-": 1
    };

    for (var i = 0; i < result.length; i++) {
        var subresult = result[i];

        var canswap

        if (subresult[1] in mapping) {
            subresult[1] = mapping[subresult[1]];
            var j = subresult[0];
            subresult[0] = subresult[2];
            subresult[2] = j;
        } else if (subresult[0] < subresult[2] && !(subresult[1] in dontswap)) {
            var j = subresult[0];
            subresult[0] = subresult[2];
            subresult[2] = j;
        }
    }

    return result;
}

function solve_numbers(numbers, target) {
    numbers.sort();
    bestresult = numbers[0];
    bestops = [];

    /* see if one of these numbers is the answer */
    for (var i = 1; i < numbers.length; i++) {
        if (Math.abs(numbers[i] - target) < Math.abs(bestresult - target)) {
            bestresult = numbers[i];
            bestops = [];
            bestvalsums = numbers[i];
        }
    }
    if (bestresult == target)
        return [bestresult, bestops];

    /* attempt to solve with iddfs */
    _recurse_solve_numbers(0, numbers, target, numbers.length, [], 0);
    bestops = tidyup_result(bestops);

    if (bestresult == target)
        return [bestresult, bestops];

    /* return best answer */
    return [bestresult, bestops];
}
