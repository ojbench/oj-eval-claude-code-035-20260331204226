#!/usr/bin/env python3
import requests
import json
import os
import sys

def test_submission():
    token = os.environ.get('ACMOJ_TOKEN')
    if not token:
        print("ERROR: ACMOJ_TOKEN not set")
        return False

    print(f"Token: {token[:10]}...")

    headers = {
        'Authorization': f'Bearer {token}',
        'Content-Type': 'application/x-www-form-urlencoded',
        'User-Agent': 'ACMOJ-Python-Client/2.2'
    }

    git_url = 'https://github.com/ojbench/oj-eval-claude-code-035-20260331204226.git'
    data = {
        'language': 'git',
        'code': git_url
    }

    url = 'https://acm.sjtu.edu.cn/OnlineJudge/api/v1/problem/2493/submit'

    print(f"URL: {url}")
    print(f"Data: {data}")
    print(f"Headers: {{'Authorization': 'Bearer ***', 'Content-Type': '{headers['Content-Type']}'}}")

    try:
        response = requests.post(url, headers=headers, data=data, timeout=30)
        print(f"\nStatus Code: {response.status_code}")
        print(f"Response Headers: {dict(response.headers)}")
        print(f"Response Body: {response.text}")

        if response.status_code in [200, 201]:
            result = response.json()
            submission_id = result.get('id')
            print(f"\n✅ SUCCESS! Submission ID: {submission_id}")
            return True
        else:
            print(f"\n❌ FAILED with status {response.status_code}")
            return False

    except Exception as e:
        print(f"\n❌ EXCEPTION: {e}")
        return False

if __name__ == "__main__":
    success = test_submission()
    sys.exit(0 if success else 1)
