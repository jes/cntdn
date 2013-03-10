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
var bestvalsums;

var OPS = {
    "+": function(n1, n2) { if (n1 < 0 || n2 < 0) return false; return n1+n2; },
    "-": function(n1, n2) { if (n2 >= n1) return false; return n1-n2; },
    "_": function(n2, n1) { if (n2 >= n1) return false; return n1-n2; },
    "*": function(n1, n2) { return n1*n2; },
    "/": function(n1, n2) { if (n2 == 0 || n1%n2 != 0) return false; return n1/n2; },
    "?": function(n2, n1) { if (n2 == 0 || n1%n2 != 0) return false; return n1/n2; },
};

function _recurse_solve_numbers(numbers, searchedi, was_generated, target, levels, valsums) {
    levels--;

    for (var i = 0; i < numbers.length-1; i++) {
        var ni = numbers[i];

        if (ni === false)
            continue;

        numbers[i] = false;

        for (var j = i+1; j < numbers.length; j++) {
            var nj = numbers[j];

            if (nj === false)
                continue;

            if (i < searchedi && !was_generated[i] && !was_generated[j])
                continue;

            for (var o in OPS) {
                var r = OPS[o](ni[0], nj[0]);
                if (r === false)
                    continue;

                var op_cost = Math.abs(r);
                while (op_cost % 10 == 0 && op_cost != 0)
                    op_cost /= 10;

                var newvalsums = valsums + op_cost;

                if ((Math.abs(r - target) < Math.abs(bestresult[0] - target))
                        || (Math.abs(r - target) == Math.abs(bestresult[0] - target) && /*Math.floor(Math.random()*10) < 1)) {*/newvalsums < bestvalsums)) {
                    bestresult = [r,ni,o,nj];
                    bestvalsums = newvalsums;
                }

                numbers[j] = [r, ni, o, nj];
                var old_was_gen = was_generated[j];
                was_generated[j] = true;

                if (levels > 0)
                    _recurse_solve_numbers(numbers, i+1, was_generated, target, levels, newvalsums);

                was_generated[j] = old_was_gen;
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

    var swappable = {
        "*": true, "+": true
    };

    if (result.length != 4)
        return result;

    result[1] = tidyup_result(result[1]);
    result[3] = tidyup_result(result[3]);

    if (result[2] in mapping) {
        result[2] = mapping[result[2]];
        var j = result[1];
        result[1] = result[3];
        result[3] = j;
    } else if (result[1][0] < result[3][0] && swappable[result[2]]) {
        var j = result[1];
        result[1] = result[3];
        result[3] = j;
    }

    return result;
}

function fullsize(array) {
    if (array.constructor != Array)
        return 0;

    var l = 0;

    for (var i = 0; i < array.length; i++)
        l += fullsize(array[i]);

    return l + array.length;
}

function serialise_result(result) {
    if (result.length != 4)
        return [];

    var leftsteps = serialise_result(result[1]);
    var rightsteps = serialise_result(result[3]);

    /* do the shortest half second */
    if (fullsize(leftsteps) < fullsize(rightsteps)) {
        var t = leftsteps;
        leftsteps = rightsteps;
        rightsteps = t;
    }

    return leftsteps.concat(rightsteps, [[result[0], result[1][0], result[2], result[3][0]]]);
}

function stringify_result(serialised, target) {
    var output = '';

    serialised = serialised.slice(0);

    var joinable = {
        "*": true, "+": true
    };

    for (var i = 0; i < serialised.length; i++) {
        var x = serialised[i];
        var y = serialised[i+1];

        if (i != serialised.length-1 && y[2] == x[2] && joinable[x[2]]) {
            if (y[1] == x[0]) {
                y[1] = x[1] + ' ' + x[2] + ' ' + x[3];
            } else if(y[3] == x[0]) {
                y[3] = x[1] + ' ' + x[2] + ' ' + x[3];
            }
        } else {
            output += x[1] + ' ' + x[2] + ' ' + x[3] + ' = ' + x[0] + '\n';
        }
    }

    var result = serialised[serialised.length-1][0];
    if (result != target)
        output += '(off by ' + (Math.abs(result - target)) + ')\n';

    return output;
}

function solve_numbers(numbers, target) {
    numbers.sort();
    bestresult = [numbers[0], numbers[0]];

    numbers = numbers.map(function(x) { return [x, false] });

    /* see if one of these numbers is the answer */
    for (var i = 1; i < numbers.length; i++) {
        if (Math.abs(numbers[i] - target) < Math.abs(bestresult[0] - target)) {
            bestresult = [numbers[i], numbers[i]];
            bestvalsums = numbers[i];
        }
    }
    if (bestresult[0] == target)
        return target + " = " + target;

    var was_generated = [];
    for (var i = 0; i < numbers.length; i++)
        was_generated.push(false);

    /* attempt to solve with dfs */
    _recurse_solve_numbers(numbers, 0, was_generated, target, numbers.length, 0);
    return stringify_result(serialise_result(tidyup_result(bestresult)), target);
}
