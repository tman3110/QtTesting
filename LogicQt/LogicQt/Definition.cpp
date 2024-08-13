#include"Definition.h"

double editDistance(wstring a, wstring b) {
	int n = (int)a.length();
	int m = (int)b.length();
	vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));


	for (int i = 0; i <= n; i++) dp[i][0] = i;
	for (int i = 0; i <= m; i++) dp[0][i] = i;

	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1];
			else dp[i][j] = min(dp[i - 1][j - 1], min(dp[i][j - 1], dp[i - 1][j])) + 1;
		}
	}
	return (double)dp[n][m];
}

void getWord(TrieNode* root, wstring def, vector<pair<double, Word>>& defSearch) {
	if (root == nullptr) return;
	if (defSearch.size() >= 6) return;

	if (root->wordEnd) {
		bool ok = false;
		double distance = 1e9;
		double temp;
		for (int i = 0; i < (int)root->word.definitions.size(); i++) {
			distance = min(distance, editDistance(def, root->word.definitions[i]));
			temp = distance / (double)max((double)def.length(), (double)root->word.definitions[i].length());
			if (temp <= 0.08) {
				ok = true;
				break;
			}
		}
		if (ok) defSearch.push_back({ temp, root->word });
	}

	for (int i = 0; i < sizeChar; i++) {
		getWord(root->childNode[i], def, defSearch);
	}
}

vector<pair<double, Word>> defSearch(Trie& trie, wstring def) {
	vector<pair<double, Word>> v;
	getWord(trie.getRoot(), def, v);
	sort(v.begin(), v.end(), [](pair<double, Word> a, pair<double, Word> b) {return a.first < b.first; });
	return v;
}