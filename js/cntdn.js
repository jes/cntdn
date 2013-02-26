/* Javascript version of cntdn
 *
 * Countdown game solver
 *
 * James Stanley 2013
 */

function is_letter(letter) {
    /* TODO: this */
    return true;
}

function _recurse_solve(letters, node, used_letter, cb, answer) {
    if (node[0])
        cb(answer);

    if (answer.length == letters.length)
        return;

    var done = {};

    for (var i = 0; i < letters.length; i++) {
        var c = letters.charAt(i);

        if (used_letter[i] || !is_letter(c) || done[c])
            continue;

        if (node[c]) {
            used_letter[i] = true;
            done[c] = true;
            _recurse_solve(letters, node[c], used_letter, cb, answer+c);
            used_letter[i] = false;
        }
    }
}

function solve_letters(letters, cb) {
    var used_letter = {};

    _recurse_solve(letters, dictionary, used_letter, cb, '');
}
