/* Javascript version of cntdn
 *
 * Countdown game solver
 *
 * James Stanley 2014
 */

function _recurse_solve_letters(letters, node, used_letter, cb, answer) {
    if (node[0])
        cb(answer, node[0]);

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

function sufficient_letters(word, letters) {
    var count = {};

    for (var i = 0; i < letters.length; i++) {
        if (!count[letters.charAt(i)])
            count[letters.charAt(i)] = 0;
        count[letters.charAt(i)]++;
    }

    for (var i = 0; i < word.length; i++) {
        if (!count[word.charAt(i)])
            return false;
        count[word.charAt(i)]--;
        if (count[word.charAt(i)] < 0)
            return false;
    }

    return true;
}

function word_in_dictionary(word) {
    var node = dictionary;
    var idx = 0;

    while (idx < word.length) {
        node = node[word.charAt(idx)];
        idx++;
        if (!node)
            return false;
    }

    if (!node[0])
        return false;
    return true;
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

var OPCOST = {
    "+": 1,
    "-": 1.05,
    "_": 1.05,
    "*": 1.2,
    "/": 1.3,
    "?": 1.3,
};

function _recurse_solve_numbers(numbers, searchedi, was_generated, target, levels, valsums, trickshot) {
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
                if ((ni[0] == 10 || nj[0] == 10) && o == '*') // HACK: multiplication by 10 is cheap
                    op_cost = 1;
                op_cost *= OPCOST[o];

                var newvalsums = valsums + op_cost;

                if ((Math.abs(r - target) < Math.abs(bestresult[0] - target))
                        || (Math.abs(r - target) == Math.abs(bestresult[0] - target) && (trickshot || newvalsums < bestvalsums))) {
                    bestresult = [r,o,ni,nj];
                    bestvalsums = newvalsums;
                }

                numbers[j] = [r, o, ni, nj];
                var old_was_gen = was_generated[j];
                was_generated[j] = true;

                if (levels > 0 && (trickshot || bestresult[0] != target || newvalsums < bestvalsums))
                    _recurse_solve_numbers(numbers, i+1, was_generated, target, levels, newvalsums, trickshot);

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

    if (result.length < 4)
        return result;

    for (var i = 2; i < result.length; i++) {
        var child = result[i];

        child = tidyup_result(child);

        if (child[1] == result[1] && swappable[result[1]]) {
            result.splice(i--, 1);
            result = result.concat(child.slice(2));
        } else {
            result[i] = child;
        }
    }

    if (result[1] in mapping) {
        result[1] = mapping[result[1]];
        var j = result[2];
        result[2] = result[3];
        result[3] = j;
    } else if (swappable[result[1]]) {
        childs = result.slice(2).sort(function(a,b) { return b[0] - a[0]; });
        for (var i = 2; i < result.length; i++)
            result[i] = childs[i-2];
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
    var childparts = [];

    for (var i = 2; i < result.length; i++) {
        var child = result[i];

        if (child.length >= 4)
            childparts.push(serialise_result(child));
    }

    childparts = childparts.sort(function(a,b) { return fullsize(b) - fullsize(a); });

    var parts = [];
    for (var i = 0; i < childparts.length; i++) {
        parts = parts.concat(childparts[i]);
    }

    var sliced = result.slice(2).map(function(l) { return l[0]; });
    var thispart = [result[0], result[1]].concat(sliced);

    return parts.concat([thispart]);
}

function stringify_result(serialised, target) {
    var output = '';

    serialised = serialised.slice(0);

    for (var i = 0; i < serialised.length; i++) {
        var x = serialised[i];

        var args = x.slice(2);
        output += args.join(' ' + x[1] + ' ') + ' = ' + x[0] + '\n';
    }

    var result = serialised[serialised.length-1][0];
    if (result != target)
        output += '(off by ' + (Math.abs(result - target)) + ')\n';

    return output;
}

function _solve_numbers(numbers, target, trickshot) {
    numbers = numbers.map(function(x) { return [x, false] });

    var was_generated = [];
    for (var i = 0; i < numbers.length; i++)
        was_generated.push(false);

    bestresult = [0, 0];

    /* attempt to solve with dfs */
    _recurse_solve_numbers(numbers, 0, was_generated, target, numbers.length, 0, trickshot);

    return bestresult;
}

function solve_numbers(numbers, target, trickshot) {
    numbers.sort();
    bestresult = [numbers[0], numbers[0]];

    /* see if one of these numbers is the answer; with trickshot you'd rather
     * have an interesting answer that's close than an exact answer
     */
    if (!trickshot) {
        for (var i = 1; i < numbers.length; i++) {
            if (Math.abs(numbers[i] - target) < Math.abs(bestresult[0] - target)) {
                bestresult = [numbers[i], numbers[i]];
                bestvalsums = numbers[i];
            }
        }
        if (bestresult[0] == target)
            return target + " = " + target;
    }

    return stringify_result(serialise_result(tidyup_result(_solve_numbers(numbers, target, trickshot))), target);
}
