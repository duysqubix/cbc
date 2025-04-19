
branch := `git rev-parse --abbrev-ref HEAD`
commit-and-push: make-commitmsg push

make-commitmsg:
  git diff --staged | sgpt --role gitcommit | tee /tmp/.commitmsg 
  git commit -F /tmp/.commitmsg

push: 
  git push origin {{branch}}
  git push mirror --mirror

run SCRIPT_NAME:
  poetry run python wherescape.py host_scripts/{{SCRIPT_NAME}}

sync:
  poetry lock --no-update --no-interaction 
  poetry install --sync --no-interaction --no-root --all-extras 

test:
  poetry run pytest -n auto