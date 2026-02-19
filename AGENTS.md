## Console command rules (avoid interactive blocking)

- Avoid commands that open an interactive pager (such as `less`).
- For `git diff`, always use non-interactive format: `git --no-pager diff ...`.
- For other git commands that may paginate (`log`, `show`, etc.), use `--no-pager` or `GIT_PAGER=cat`.
- Always prefer commands that finish on their own and return complete output without requiring interactive scrolling.
