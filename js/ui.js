function solve() {
    var letters = document.getElementById('letters').value;

    var result = [];

    solve_letters(letters, function(word) { result.push(word); });

    result.sort(function(a, b) { return b.length - a.length; });

    document.getElementById('answer').value = result.join("\n");
}
